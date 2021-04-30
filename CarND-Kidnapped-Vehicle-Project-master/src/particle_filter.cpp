#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "particle_filter.h"

using namespace std;

static int NUM_PARTI = 100;

using std::string;
using std::vector;

void ParticleFilter::init(double x, double y, double theta, double std[])
{

  std::normal_distribution<double> dist_x(x, std[0]);
  std::normal_distribution<double> dist_y(y, std[1]);
  std::normal_distribution<double> dist_theta(theta, std[2]);
  std::default_random_engine gen;

  num_particles = NUM_PARTI;
  particles.resize(num_particles);
  //weights.resize(num_particles);

  for (auto &p : particles)
  {
    p.x = dist_x(gen);
    p.y = dist_y(gen);
    p.theta = dist_theta(gen);
    p.weight = 1;
  }

  is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[],
                                double velocity, double yaw_rate)
{
  std::default_random_engine gen;

  std::normal_distribution<double> N_x(0, std_pos[0]);
  std::normal_distribution<double> N_y(0, std_pos[1]);
  std::normal_distribution<double> N_theta(0, std_pos[2]);

  for (auto &p : particles)
  {

    if (fabs(yaw_rate) < 0.0001)
    {
      p.x += velocity * delta_t * cos(p.theta);
      p.y += velocity * delta_t * sin(p.theta);
    }
    else
    {
      p.x += velocity / yaw_rate * (sin(p.theta + yaw_rate * delta_t) - sin(p.theta));
      p.y += velocity / yaw_rate * (cos(p.theta) - cos(p.theta + yaw_rate * delta_t));
      p.theta += yaw_rate * delta_t;
    }

    p.x += N_x(gen);
    p.y += N_y(gen);
    p.theta += N_theta(gen);
  }
}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted,
                                     vector<LandmarkObs> &observations)
{

  for (auto &obs : observations)
  {
    double minD = std::numeric_limits<float>::max();

    for (const auto &pred : predicted)
    {
      double distance = dist(obs.x, obs.y, pred.x, pred.y);
      if (minD > distance)
      {
        minD = distance;
        obs.id = pred.id;
      }
    }
  }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[],
                                   const vector<LandmarkObs> &observations,
                                   const Map &map_landmarks)
{
  for (auto &p : particles)
  {
    p.weight = 1.0;

    vector<LandmarkObs> predictions;
    for (const auto &lm : map_landmarks.landmark_list)
    {
      double distance = dist(p.x, p.y, lm.x_f, lm.y_f);
      if (distance < sensor_range)
      {
        predictions.push_back(LandmarkObs{lm.id_i, lm.x_f, lm.y_f});
      }
    }

    vector<LandmarkObs> observations_map;
    double cos_theta = cos(p.theta);
    double sin_theta = sin(p.theta);

    for (const auto &obs : observations)
    {
      LandmarkObs tmp;
      tmp.x = obs.x * cos_theta - obs.y * sin_theta + p.x;
      tmp.y = obs.x * sin_theta + obs.y * cos_theta + p.y;
      observations_map.push_back(tmp);
    }

    dataAssociation(predictions, observations_map);

    for (const auto &obs_m : observations_map)
    {

      Map::single_landmark_s landmark = map_landmarks.landmark_list.at(obs_m.id - 1);
      double x_term = pow(obs_m.x - landmark.x_f, 2) / (2 * pow(std_landmark[0], 2));
      double y_term = pow(obs_m.y - landmark.y_f, 2) / (2 * pow(std_landmark[1], 2));
      double w = exp(-(x_term + y_term)) / (2 * M_PI * std_landmark[0] * std_landmark[1]);
      p.weight *= w;
    }

    weights.push_back(p.weight);
  }
}

void ParticleFilter::resample()
{

  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dist(weights.begin(), weights.end());

  vector<Particle> resampled_particles;
  resampled_particles.resize(num_particles);

  for (int i = 0; i < num_particles; i++)
  {
    int idx = dist(gen);
    resampled_particles[i] = particles[idx];
  }

  particles = resampled_particles;

  weights.clear();
}

void ParticleFilter::SetAssociations(Particle &particle,
                                     const vector<int> &associations,
                                     const vector<double> &sense_x,
                                     const vector<double> &sense_y)
{

  particle.associations = associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best)
{
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length() - 1);
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord)
{
  vector<double> v;

  if (coord == "X")
  {
    v = best.sense_x;
  }
  else
  {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length() - 1);
  return s;
}