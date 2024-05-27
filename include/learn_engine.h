#pragma once

#include <algorithm>
#include <random>

#include "db.h"

namespace learn_engine {

	struct StudyOptions {
		std::string deck_name = "";
		std::string category = "";
		bool random_order = false;
		bool answer_first = false;
		bool loop_forever = false;
	};

	/**
	 * Using the supplied study options, generate an acceptable set of 
	 * flash cards to be studied
	*/
	std::vector<db::FlashCard> GenLearningDeck(StudyOptions &options)
	{
		std::vector<db::FlashCard> result;

		// grab from db
		result = db::GetCards(options.deck_name, options.category);

		// randomize order
		if (options.random_order) {
			// https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
			auto rd = std::random_device{};
			auto rng = std::default_random_engine{ rd() };
			std::shuffle(std::begin(result), std::end(result), rng);
		}

		// answer first (swap question/answer fields for cards before return result)
		if (options.answer_first) {
			for (int i = 0; i < result.size(); i++) {
				std::string question = result[i].question;
				result[i].question = result[i].answer;
				result[i].answer = question;
			}
		}

		return result;
	}
};