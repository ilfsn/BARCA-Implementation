#include "BID.h"
#include <math.h>  
// DEADLOCK = 2 - LIVELOCK = 3 - THRASHING = 4 - MEMLEAK = 5
std::random_device seed;
// generator 
std::mt19937 engine(seed());

double kernel(std::vector<double> xi, std::vector<double> xj, double gamma)
{
	double ret = 0;
	for (int k = 0; k < xi.size(); k++) {
		ret += pow(xi[k] - xj[k], 2);
	}
	ret = exp(-gamma * ret);
	return ret;
}

int two_step::update(int fb)
{
	switch (fb)
	{
	case -1:
		one_cler.UPDATE_MODEL(one_cler.Latest);
		binary_cler.BALANCED_UPDATE(binary_cler.Latest, -1);
		break;
	case 2:
		binary_cler.BALANCED_UPDATE(binary_cler.Latest, 1);
		for (auto v : dltm.D) {
			v->BALANCED_UPDATE(v->Latest, 1);
		}
		break;
	case 3:
		binary_cler.BALANCED_UPDATE(binary_cler.Latest, 1);
		for (auto v : dltm.L) {
			v->BALANCED_UPDATE(v->Latest, 1);
		}
		break;
	case 4:
		binary_cler.BALANCED_UPDATE(binary_cler.Latest, 1);
		for (auto v : dltm.T) {
			v->BALANCED_UPDATE(v->Latest, 1);
		}
		break;
	case 5:
		binary_cler.BALANCED_UPDATE(binary_cler.Latest, 1);
		for (auto v : dltm.M) {
			v->BALANCED_UPDATE(v->Latest, 1);
		}
		break;
	default:
			break;
	}
	return 0;
}

int two_step::predict(std::vector<double> feature_vector)
{
	
	if ((one_cler.CLASSIFY(feature_vector) == 1) | (binary_cler.CLASSIFY(feature_vector) == 1)) {
		return dltm.CLASSIFY(feature_vector);
	}
	return -1;
}

std::vector<double> one_class::NORMALIZE(std::vector<double> xj)
{
	std::vector<double> ret;
	if (mu.size() == 0) mu = xj;
	for (int idx = 0; idx < dimension; idx++) {
		double acc = 1 / (1 + exp((xj[idx] - mu[idx]) / sigma[idx]));
		ret.push_back(acc);
	}

	return ret;
}

int one_class::NORM_UPDATE()
{
	double test;
	for (int idx = 0; idx < dimension; idx++) {
		mu[idx] = (1 - 1 / t)*mu[idx] + 1 / t * Latest[idx];
		sigma[idx] = (1 - 1 / t)*sigma[idx] + (1 / t) * pow((Latest[idx] - mu[idx]), 2);
		if (sigma[idx] == 0) sigma[idx] += 0.01;
	}
	return 0;
}

int one_class::CLASSIFY(std::vector<double> xj)
{

	Latest = xj;
	t++;
	std::vector<double> norml_v = NORMALIZE(xj);
	//NORM_UPDATE();

	if (V.empty() && alpha.empty()) {
		V.push_back(norml_v);
		alpha.push_back(eta);
	}

	double acc = 0;
	for (int idx = 0; idx < alpha.size(); idx++) {
		acc += alpha[idx] * kernel(V[idx], norml_v, gamma);
	}
	if (rho<acc) return -1;
	else return 1;
}

int one_class::UPDATE_MODEL(std::vector<double> xj)
{
	for (auto al:alpha) {
		al = (1 - nu)*al;
	}

	std::vector<double> norml_v = NORMALIZE(xj);
	NORM_UPDATE(); // Note position of NORM UPDATE
	double acc = 0;
	for (int idx = 0; idx < alpha.size(); idx++) {
		acc += alpha[idx] * kernel(V[idx], norml_v, gamma);
	}
	if (acc < rho) {
		V.push_back(norml_v);
		alpha.push_back(eta); 
		rho = rho - rho * eta * (1 - nu);
	}
	else rho = rho + rho * eta * nu;
	
	return 0;
}

int binary_class::NORM_UPDATE()
{
	for (int idx = 0; idx < dimension; idx++) {
		mu[idx] = (1 - 1 / t)*mu[idx] + 1 / t * Latest[idx];
		sigma[idx] = (1 - 1 / t)*sigma[idx] + 1 / t * pow((Latest[idx] - mu[idx]), 2);
		if (sigma[idx] == 0) sigma[idx] += 0.01;
	}
	return 0;
}

std::vector<double> binary_class::NORMALIZE(std::vector<double> xj)
{
	std::vector<double> ret;
	if (mu.size() == 0) mu = xj;
	for (int idx = 0; idx < dimension; idx++) {
		double acc = 1 / (1 + exp((xj[idx] - mu[idx]) / sigma[idx]));
		ret.push_back(acc);
	}

	return ret;
}


int binary_class::CLASSIFY(std::vector<double> xj)
{
	
	Latest = xj;
	t++;
	std::vector<double> norml_v = NORMALIZE(xj);
	//NORM_UPDATE();

	if (V.empty() && alpha.empty()) {
		V.push_back(norml_v);
		alpha.push_back(eta);
	}

	double acc = 0;
	for (int idx = 0; idx < alpha.size(); idx++) {
		acc += alpha[idx] * kernel(V[idx], norml_v, gamma);
	}
	if (acc<0) return -1;
	else return 1;	
}

int binary_class::error_code(int left, int right)
{
	errc_left = left;
	errc_righ = right;
	return 0;
}

int binary_class::CLASSIFYmulti(std::vector<double> xj)
{
	if (this->leaf == false) {
		switch (CLASSIFY(xj)) {
		case -1:			
			return left->CLASSIFYmulti(xj);			
		case 1:
			return right->CLASSIFYmulti(xj);
		}		
	}
	else {
		switch (CLASSIFY(xj))
		{
		case -1:
			return errc_left;
		case 1:
			return errc_righ;
		default:
			break;
		}
	}
}

int binary_class::UPDATE_MODEL(std::vector<double> xj, int yj)
{
	for (auto al : alpha) {
		al = (1 - lambda * eta)*al;
	}

	double acc = 0;
	for (int idx = 0; idx < alpha.size(); idx++) {
		acc += alpha[idx] * kernel(V[idx], xj, gamma);
	}
	if (yj*acc <= rho) {
		alpha.push_back(eta*yj);
		V.push_back(xj);
		// ? yi
	}
	return 0;
}

int binary_class::BALANCED_UPDATE(std::vector<double> xj, int yj)
{

	// number distribution
	
	/*if (!P.empty()) 
		std::uniform_int_distribution<int> chooseP(0, P.size() - 1);
	if (!N.empty()) 
		std::uniform_int_distribution<int> chooseN(0, N.size() - 1);*/
	
	std::vector<double> norml_v = NORMALIZE(xj);
	NORM_UPDATE(); // Note position of NORM UPDATE

	if (V.empty()) {
		UPDATE_MODEL(norml_v, yj);
	}
	else if (yj < 0) {
		if (P.size() > 0) {
			std::uniform_int_distribution<int> chooseP(0, P.size() - 1);
			UPDATE_MODEL(norml_v, yj);
			UPDATE_MODEL(P[chooseP(engine)], 1);
		}
		else
			N.push_back(norml_v);
	}
	else {
		if (N.size() > 0) {
			std::uniform_int_distribution<int> chooseN(0, N.size() - 1);
			UPDATE_MODEL(norml_v, yj);
			UPDATE_MODEL(N[chooseN(engine)],-1);
		}
		else
			P.push_back(norml_v);
	}
	
	return 0;
}


int multi_classDLTM::CLASSIFY(std::vector<double> feature_vector)
{
	return DM.CLASSIFYmulti(feature_vector);	
}