#pragma once

#include <functional>

#include "ftxui/component/component.hpp"	  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"			  // for separator, gauge, text, Element, operator|, vbox, border

#include "gui_custom.h"
#include "db.h"
#include "learn_engine.h"

using namespace ftxui;

namespace pages
{
	

	/* forward declares */

	void HomePage();
	void BrowseDecksPage();
	void SettingsPage();
	void PreLearnPage();
	void LearnPage();

	/* variables */

	bool quit_request = false;
	std::function<void()> next_page = HomePage;
	learn_engine::StudyOptions current_study_options;

	// main loop for pages
	void run()
	{
		while (!quit_request)
		{
			next_page();
		}
	}

	/** Page rendering functions below */

	/**
	 * A dummy to make sure thread stuff does not prevent
	 * exiting when desired. If you set this as 
	 * next page, it should quit and dump like you want
	 * no matter what.
	*/
	void QuitPage()
	{
		quit_request = true;
	}

	/**
	 * List of initial choices:
	 * - Browse decks to practice
	 * - Start new deck
	 * - Options
	 * - Import
	 * - Export
	*/
	void HomePage()
	{
		auto screen = ScreenInteractive::Fullscreen();

		const std::vector<std::string> menu_choices = {
			"Browse Decks",
			"Start New Deck",
			"Options",
			"Import",
			"Export",
		};
		int selected = 0;
		auto main_menu = Menu(
			&menu_choices,
			&selected
		);

		auto all_components = Container::Vertical({
			main_menu,
		});

		std::string message = "";

		// TODO: change next_page to appropriate functions as they are written
		auto doc = Renderer(all_components, [&]
		{
			switch (selected)
			{
				case 0:
					next_page = BrowseDecksPage;
					message = "Time to learn!";
					break;
				case 1:
					next_page = QuitPage;
					message = "New set of cards ahead.";
					break;
				case 2:
					next_page = SettingsPage;
					message = "Change settings about flash cards.";
					break;
				case 3:
					next_page = QuitPage;
					message = "Import a db of decks.";
					break;
				case 4:
					next_page = QuitPage;
					message = "Export your db of decks.";
					break;
				default:
					message = "Sup.";
			}
		
			return vbox({
				vbox({
					text("Welcome to Wally's flash cards") | center,
					separator(),
					main_menu->Render(),
					separator(),
					hbox({
						filler(),
						text(message),
						filler(),
					}),
					separator(),
					gui_custom::BottomHelpText(),
				}) | border,
			}); 
		});

		// Make mods to renderer to capture key presses as needed
		doc |= CatchEvent([&](Event event) {
			if (event == Event::Character('\n')) {
				screen.Exit(); // force change to next page
				return true;
			}
			else if (event == Event::Character('q') || event == Event::Backspace) {
				quit_request = true;
				screen.Exit();
				return true;
			}

			return false;
		});

		screen.Loop(doc);
	}

	void BrowseDecksPage()
	{
		auto screen = ScreenInteractive::Fullscreen();

		const std::vector<std::string> menu_choices = db::GetDecks();

		int selected = 0;
		auto main_menu = Menu(
			&menu_choices,
			&selected
		);

		// auto buttons = Container::Horizontal({
		// 	exit_button,
		// 	go_button
		// });

		auto all_components = Container::Vertical({
			main_menu,
			// buttons
		});

		std::string message = "";

		auto doc = Renderer(all_components, [&]
		{
			message = "Press enter/go to study the deck, \"" + menu_choices[selected] + "\"";
			current_study_options.deck_name = menu_choices[selected];

			return vbox({
				vbox({
					text("Choose a deck from which to study") | center,
					separator(),
					main_menu->Render(),
					separator(),
					hbox({
						filler(),
						text(message),
						filler(),
					}),
					separator(),
					gui_custom::BottomHelpText(),
				}) | border,
			});
		});

		doc |= CatchEvent([&](Event event) {
			if (event == Event::Character('\n')) {
				next_page = PreLearnPage;
				screen.Exit();
				return true;
			} else if (event == Event::Character('q')) {
				next_page = QuitPage;
				screen.Exit();
				return true;
			} else if (event == Event::Backspace) {
				next_page = HomePage;
				screen.Exit();
				return true;
			}

			return false;
		});

		screen.Loop(doc);
	}

	/**
	 * Set options about studying the set with a learn_engine::StudyOptions struct.
	*/
	void PreLearnPage()
	{
		auto screen = ScreenInteractive::Fullscreen();

		// category choices setup
		std::vector<std::string> category_choices = db::GetCategories(current_study_options.deck_name);
		category_choices.push_back("All");
		int selected = category_choices.size()-1;
		auto category_choose_menu = Menu(
			&category_choices,
			&selected
		);
		auto categories_container = Container::Vertical({
			category_choose_menu,
		});

		// additional on/off study options setup
		const std::vector<std::string> study_bool_options = {
			"Randomize practice order",
			"Flip the cards and present the answer side first      ",
			"Loop the deck forever until quit out"
		};
		std::array<bool, 3> study_bool_flags = {false, false, false};
		auto flags_container = Container::Vertical({
			Checkbox(&study_bool_options[0], &study_bool_flags[0]),
			Checkbox(&study_bool_options[1], &study_bool_flags[1]),
			Checkbox(&study_bool_options[2], &study_bool_flags[2]),
		});

		auto go_button = Button(
			"Start practicing",
			[&] {
				// TODO: change to LearnPage when implemented
				next_page = LearnPage;
				screen.Exit();
			},
			ButtonOption::Animated(Color::Aquamarine1)
		);

		// aggregate containers so can supply to Renderer
		auto all_components = Container::Horizontal({
			categories_container,
			flags_container,
			go_button
		});

		// TODO: scrollable card preview to take up rest of space below?

		std::string message = "";

		auto doc = Renderer(all_components, [&]
		{
			message = "Set your options and learn!";

			// update options
			current_study_options.random_order = study_bool_flags[0];
			current_study_options.answer_first = study_bool_flags[1];
			current_study_options.loop_forever = study_bool_flags[2];
			/**
			 * If first option selected, "All", only give empty str for category.
			 * In the db call, it won't filter for category at all and return 
			 * ALL the cards in the deck.
			*/
			current_study_options.category = (selected == 0) ? "" : category_choices[selected];

			auto category_win = window(
				text("Categories"),
				vbox({
					text("Choose category to filter cards by, or study all in deck") | center | size(WIDTH, EQUAL, 80),
					text(""),
					categories_container->Render(),
					text(""),
				})
			);

			auto flags_win = window(
				text("Extra"),
				vbox({
					text("Set extra flags about how to present the cards"),
					text(""),
					flags_container->Render(),
					text(""),
				})
			);

			return vbox({
				vbox({
					hbox({
						category_win,
						flags_win,
						go_button->Render() | center | flex,
					}),
					separator(),
					gui_custom::BottomHelpText(),
				}) | border,
			});
		});

		doc |= CatchEvent([&](Event event) {
			if (event == Event::Character('q')) {
				next_page = QuitPage;
				screen.Exit();
				return true;
			} else if (event == Event::Backspace) {
				next_page = BrowseDecksPage;
				screen.Exit();
				return true;
			}

			return false;
		});

		screen.Loop(doc);
	}

	void LearnPage()
	{
		auto screen = ScreenInteractive::Fullscreen();

		auto questions = learn_engine::GenLearningDeck(current_study_options);
		auto cur_card = questions[0];
		int cur_q_index = 0;
		int num_answered = 0;
		float progress = 0.0f;
		bool show_answer = false;

		auto all_components = Container::Vertical({});

		auto doc = Renderer(all_components, [&] {
			std::string answer_text = (show_answer) ? cur_card.answer : "Press ENTER to show answer!";
			progress = (float)num_answered / questions.size();

			auto question_window = window(
				text("Question"),
				hbox({
					filler(),
					text(cur_card.question),
					filler(),
				})
			);

			auto answer_window = window(
				text("Answer"),
				hbox({
					filler(),
					text(answer_text),
					filler(),
				})
			);

			return vbox({
				hbox({ 		// TODO: question/answer panels
					question_window,
					separator(),
					answer_window | size(WIDTH, GREATER_THAN, 30),
				}) | size(HEIGHT, EQUAL, 40),
				gauge(progress),
				separator(),
				gui_custom::BottomLearningHelpText(current_study_options.deck_name),
				separator(),
			}) | border;
		});

		doc |= CatchEvent([&](Event event) {
			if (event == Event::Character('\n')) {
				if (!show_answer) { 			// show the hidden answer
					show_answer = true;
					num_answered++;
					return true;
				} else { 									// answer is already showing
					if (cur_q_index < questions.size()-1) { 	// more questions are left, show next one
						cur_q_index++;
						cur_card = questions[cur_q_index];
					} else {								// no questions left, either regen or exit
						if (current_study_options.loop_forever) {
							// deck exhausted, loop back and continue
							num_answered = 0;
							cur_q_index = 0;
							cur_card = questions[cur_q_index];
						} else {
							// exit, deck exhausted
							next_page = PreLearnPage;
							screen.Exit();
							return true;
						}
					}
					show_answer = false;
					return true;
				}
			} else if (event == Event::Character('q')) {
				next_page = QuitPage;
				screen.Exit();
				return true;
			} else if (event == Event::Backspace) {
				next_page = PreLearnPage;
				screen.Exit();
				return true;
			}

			return false;
		});

		screen.Loop(doc);
	}

	// TODO: get some inputs in here or something; chart out what you can change and whatnot
	void SettingsPage()
	{
		auto screen = ScreenInteractive::Fullscreen();

		auto back_button = Button(
			"Back to main menu " + gui_custom::symbols::left_arrow,
			[&]{next_page = HomePage; screen.Exit();}
		);	

		auto end_container = Container::Vertical({
			back_button
		});

		auto doc = Renderer(back_button, [&] {
			return vbox({
				hbox({
					back_button->Render(),
				})
			});
		});

		screen.Loop(doc);
	}
};
