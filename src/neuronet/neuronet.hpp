

/*

   **************************************************************
   *                                                            *
   *  neuronet                                                  *
   *                                                            *
   *  Sujeet Akula                                              *
   *  sujeet@freeboson.org                                      *
   *                                                            *
   *  Artificial Neural Network implementation with one         *
   *  hidden layer and a single output node and back-           *
   *  propagation of errors--designed for approximating         *
   *  likelihood surfaces of complex problems                   *
   *                                                            *
   **************************************************************

*/

#pragma once

#ifndef _NEURONET_H_
#define _NEURONET_H_

#include <array>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>


template <int input_nodes, int hidden_nodes>
class neuronet
{
public:
	neuronet();

	long double approx(const std::array<long double, input_nodes> &input) const;
	bool learn(const std::array<long double, input_nodes> &input, long double actual, long double *err = nullptr);

private:
	std::array<long double, hidden_nodes> hidden_biases;
	std::array<std::array<long double, input_nodes>, hidden_nodes> hidden_weights;  // this is an array of arrays of weights to hidden nodes
										 	// i.e., hidden_weights[i] is the array of weights of connections 
											// from all of the input nodes to the ith hidden node
	
	long double final_bias;
	std::array<long double, hidden_nodes> final_weights;

	void randomize_weights();

};

template <int input_nodes, int hidden_nodes>
long double neuronet::approx(const std::array<long double, _input_nodes> &input) const
{
	std::array<long double, hidden_nodes> hidden_node_vals;
	std::transform(
		hidden_weights.begin(),
		hidden_weights.end(),
		hidden_biases.begin(),
		hiddeen_node_vals.begin(),
		[&input](std::array<long double,input_nodes> weights, long double bias) -> long double {
			return std::tanh(std::inner_product(input.begin(),input.end(),weights.begin(),bias));
		}
	);
	
	return std::inner_product(hidden_node_vals.begin(),hidden_node_vals.end(),final_weights.begin(),final_bias);
}

#endif

