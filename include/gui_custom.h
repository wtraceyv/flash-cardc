#pragma once

#include <string>

#include "ftxui/dom/elements.hpp"			  // for separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;

namespace gui_custom
{
	namespace symbols
	{
		const std::string left_arrow = "🠈";
		const std::string right_arrow = "🠊";
		const std::string enter = "⏎";
	};

	/** forward declare */

	Element GTRBanner();

	/** funciones */

	inline Element BottomLearningHelpText(std::string deck_name)
	{
		return vbox({
			vbox({
				filler(),
				text("You are now practicing from the deck, \'" + deck_name + "'. Your progress may be indicated by a gauge at the bottom of the page."),
				text(""),
				filler(),
			}),
			vbox({
				filler(),
				text("Press ENTER to show the answer or advance to the next flash card."),
				text("Press q to quit the program."),
				text("Press backspace to return to options page."),
				filler(),
			}),
		});
	}

	inline Element BottomHelpText()
	{
		return vbox({
			// GTRBanner(),
			hbox({
				filler(),
				text("Press Enter to choose an option."),
				filler(),
			}),
			hbox({
				filler(),
				text("Press q to quit the program."),
				filler(),
			}),
			hbox({
				filler(),
				text("Press backspace to go to previous page.") | size(HEIGHT, EQUAL, 30),
				filler(),
			}),
		});
	}

	inline Element GTRBanner()
	{
		return vbox({
			text("                                             ⣀⣀⣀⣀⣠⣤⣤⣤⠶⠶⠶⢦⣤⣤⣤⣄⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣤⠤⠤⠤⢤⣤⣤⣤⣤⣄⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
			text("                                             ⣟⠛⠿⢭⣛⣉⠉⠉⠉⠉⠉⠉⠙⢿⡁⠀⠀⠉⠉⠉⠉⠛⣦⠤⠖⠒⠚⠛⠛⠛⠛⠛⢓⣶⠶⠖⠚⠉⢙⣁⣭⠭⠿⠛⠛⠛⠻⢶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
			text("                                             ⢽⣄⢀⣠⡴⠛⠉⠉⠉⠉⠻⡗⠚⢻⡇⠀⠀⠀⠀⠀⣠⡴⠋⠀⠀⠀⠀⠀⢀⣠⠴⠚⠉⠀⠤⢤⡶⠊⠉⠀⠹⡄⠀⠀⠀⠀⠀⠀⠉⠻⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
			text("                                             ⠀⠉⠉⠀⠀⠀⠀⢀⣤⣴⣾⣥⣶⡾⣷⣀⣀⣠⣴⣿⠥⠤⣄⣀⣀⣀⡤⠖⠉⠀⠀⠀⠀⠀⠀⡜⠀⠀⠀⠀⠀⢹⣄⣀⣀⣀⣀⣀⣀⣀⣀⣹⣿⣶⣶⣤⣤⣀⡀⠀⠀⠀⠀⠀"),
			text("                                             ⠀⠀⠀⠀⠀⣰⠟⠻⠯⠥⣄⣄⣿⠓⠛⡛⢉⣭⣤⣤⣤⠤⠴⠚⠛⠁⠀⠀⠀⠈⠉⠉⠉⠉⠙⠛⠉⠉⠉⠉⠉⠉⣿⡁⠀⠀⠀⠀⢀⣀⣀⣀⣀⣉⣧⣀⢉⡽⠛⠛⢳⣦⡄⠀"),
			text("                                             ⠀⠀⠀⠀⢰⡿⣄⡀⠀⠀⠀⠀⢉⣹⡿⢻⣿⠿⣿⣇⡉⣑⣦⣀⣀⣀⡤⠤⠤⣤⣤⡶⠶⠶⠶⠷⠶⢾⣉⠉⠉⠉⠙⡏⠉⠉⠉⠉⠉⠉⠁⠀⠀⠈⢹⢻⣿⠇⠀⣴⣿⣿⣿⣿"),
			text("                                             ⠀⠀⠀⢠⡿⠀⠀⠉⠉⠙⠒⣶⡟⢉⣿⡿⠁⠀⢸⣿⠋⠉⣿⠀⠀⠀⢀⡤⠞⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠲⡄⠀⠸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡟⠀⢰⣿⣿⣿⣿⢻"),
			text("                                             ⠀⠀⠀⢸⡷⣦⣀⡀⠀⠀⠘⢿⣧⠞⢫⣷⣄⣠⠏⣸⠀⠀⡏⠀⢀⡴⠋⠀⠀⠀⠀⠀⢀⣴⣶⣶⣶⡦⣄⡀⠀⠈⢦⠀⢧⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡿⠀⠀⣿⣿⣿⣾⣿⣴"),
			text("                                             ⠀⠀⠀⢸⣷⡇⠀⠉⠑⣶⠀⠀⠀⠀⠀⠉⠉⠀⠐⡇⠀⢸⡇⣠⠟⠀⠀⠀⠀⠀⣠⣾⣿⡟⢀⣽⣧⡹⣟⣷⡀⠀⠈⣧⠸⡄⠀⠀⠀⠀⢀⣀⣠⣼⣿⠃⠀⢀⡇⠻⣿⣿⠟⠛"),
			text("                                             ⠀⠀⠀⢸⡿⢷⣄⡀⢀⡇⠀⣀⣀⣀⣀⣀⣀⠀⢀⠇⠀⠈⢻⡟⠲⢶⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⠟⢷⢸⣹⣷⠀⠀⠘⣆⣧⣠⢤⣶⣾⣿⣿⣷⣿⣿⠤⠴⠚⠉⠉⠉⠁⠀⠀"),
			text("                                             ⠀⠀⠀⢸⣿⣦⣍⡛⠻⠃⡜⠉⠉⠀⠈⠉⢹⡆⢸⠀⠀⠀⠈⢧⡀⠀⠀⢀⡝⢉⣿⣿⣿⣿⣿⣅⡀⣸⢻⢿⣿⠀⠀⠀⢹⡿⢷⣾⡿⠿⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
			text("                                             ⠀⠀⠀⠻⣿⣿⢿⣿⣷⣶⣧⣄⣀⣀⠀⠀⢸⡇⢸⠀⠀⠀⠀⠀⠉⠑⠲⡞⠀⠀⣿⣿⣿⡿⠿⣿⣿⠇⣼⡾⣹⠀⠀⣀⠼⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
			text("                                             ⠀⠀⠀⠀⠈⠻⢶⣭⣛⣻⣿⣷⡾⢿⣿⣿⣿⣷⣿⡦⠤⣤⣤⣀⣀⣠⣼⡇⠀⠀⠹⣿⣿⣿⠀⡨⢏⣼⣿⣧⣧⠴⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
			text("                                             ⠀⠀⠀⠀⠀⠀⠀⠘⠻⢯⣉⠙⣷⣼⣿⣇⣳⣿⠈⢧⠀⠸⣄⡰⠋⠀⠀⣧⣄⡀⠀⠈⠻⠽⢯⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
			text("                                             ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠛⠿⠿⠿⢧⣬⣷⣶⣞⣁⣤⣤⣤⡵⠀⠉⠙⠒⠒⠛⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"),
		});
	}

};
