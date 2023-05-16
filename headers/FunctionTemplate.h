#pragma once
#include<fplus/fplus.hpp>
#include<iostream>

template<typename InputType, typename OutputType>
class FunctionalPipeline {
public:
	typedef std::function<OutputType(InputType)> Step;

	FunctionalPipeline(const Step& step) :steps_({ step }) {}

	FunctionalPipeline<InputType, OutputType>& add_step(const Step& step) {
		steps_.push_back(steps);
		return *this;
	}

	OutputType process(const InputType& input) const {
		return fplus::apply(input, steps_);
	}
private:
	std::vector<Step> steps_;
};