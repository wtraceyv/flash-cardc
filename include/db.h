#pragma once

#include <string>
#include <format>
#include <iostream>
#include <vector>

#include <sqlite3.h>

#define db_filepath "decks.db"

/**
 * Schema?:
 * 
 * One table has all flashcards, decks differentiated by name
 * 
 * INT					id
 * TEXT					deck_name
 * TEXT 				category
 * TEXT NOT NULL		question
 * TEXT NOT NULL		answer
 * DATETIME				last_answered
 * DATETIME				next_study_rec
 * 
*/

namespace db
{
	struct FlashCard {
		int id;
		std::string deck_name;
		std::string category = "";
		std::string question;
		std::string answer;
		long last_answered = 0;
		long next_study_rec = 0;
	};

	void CreateDeckTable()
	{
		sqlite3* db;
		char* err_message;

		// TODO: create own table
		std::string sql = R"(
			CREATE TABLE IF NOT EXISTS flash_cards (
				id 				INTEGER PRIMARY KEY,
				deck_name		TEXT NOT NULL,
				category		TEXT,
				question		TEXT NOT NULL,
				answer			TEXT NOT NULL,
				last_answered	INTEGER,
				next_study_rec	INTEGER
			);
		)";

		int err = sqlite3_open(db_filepath, &db);
		err = sqlite3_exec(db, sql.c_str(), NULL, 0, &err_message);

		if (err != SQLITE_OK) {
			// TODO: have to hook this up to fxtui to display somewhere (message thing at bottom of page probably)
			printf("SQL: Error creating new deck table: %s", err_message);
			sqlite3_free(err_message);
		}

		sqlite3_close(db);
	}

	/**
	 * Insert a new flash card. If the deck_name is new, it trivially "creates" that deck in the background.
	*/
	void InsertCard(FlashCard new_card)
	{
		sqlite3* db;
		char* err_message;

		constexpr std::string_view sql_raw = R"(
			INSERT INTO flash_cards (deck_name, question, answer, category, next_study_rec)
			VALUES ('{}', '{}', '{}', '{}', {});
		)";
		std::string sql = std::format(sql_raw, new_card.deck_name, new_card.question, new_card.answer, new_card.category, new_card.next_study_rec);

		sqlite3_open(db_filepath, &db);
		int err = sqlite3_open(db_filepath, &db);
		err = sqlite3_exec(db, sql.c_str(), NULL, 0, &err_message);

		if (err != SQLITE_OK) {
			// TODO: have to hook this up to fxtui to display somewhere (message thing at bottom of page probably)
			printf("SQL: Error inserting flash card: %s", err_message);
			sqlite3_free(err_message);
		}

		sqlite3_close(db);
	}
	
	/**
	 * Get list of all distinct decks of flash cards available.
	 * 
	 * https://stackoverflow.com/questions/31146713/sqlite3-exec-callback-function-clarification/31168999#31168999
	 * https://stackoverflow.com/questions/61794729/how-does-prepared-statements-in-sqlite-c-work
	*/
	std::vector<std::string> GetDecks()
	{
		std::vector<std::string> result;
		sqlite3* db;
		sqlite3_stmt* statement;
		std::string sql = R"(
			SELECT DISTINCT deck_name
			FROM flash_cards;
		)";

		sqlite3_open(db_filepath, &db);
		int err = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL);
		if (err != SQLITE_OK) {
			// TODO: have to hook this up to fxtui to display somewhere (message thing at bottom of page probably)
			printf("SQL Error preparing statement: %s\n", sqlite3_errmsg(db));
			sqlite3_free(statement);
			return result;
		}

		while ((err = sqlite3_step(statement)) == SQLITE_ROW) {
			std::string deck_name = (char*)sqlite3_column_text(statement, 0);
			result.push_back(deck_name);
		}

		if (err != SQLITE_DONE)
			std::cout << "Error parsing SELECT: " << sqlite3_errmsg(db) << std::endl;
		
		sqlite3_finalize(statement);
		sqlite3_close(db);

		return result;
	}

	/**
	 * Get a list of distinct categories you could filter a deck's cards by.
	*/
	std::vector<std::string> GetCategories(std::string deck_name)
	{
		std::vector<std::string> result;
		sqlite3* db;
		sqlite3_stmt* statement;
		constexpr std::string_view sql_raw = R"(
			SELECT DISTINCT category
			FROM flash_cards
			WHERE deck_name = '{}';
		)";
		std::string sql = std::format(sql_raw, deck_name);

		sqlite3_open(db_filepath, &db);
		int err = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL);
		if (err != SQLITE_OK) {
			// TODO: have to hook this up to fxtui to display somewhere (message thing at bottom of page probably)
			printf("SQL Error preparing statement: %s\n", sqlite3_errmsg(db));
			sqlite3_free(statement);
			return result;
		}

		while ((err = sqlite3_step(statement)) == SQLITE_ROW) {
			std::string found_category = (char*)sqlite3_column_text(statement, 0);
			if (!found_category.empty())
				result.push_back(found_category);
		}

		if (err != SQLITE_DONE)
			std::cout << "Error parsing SELECT: " << sqlite3_errmsg(db) << std::endl;
		
		sqlite3_finalize(statement);
		sqlite3_close(db);

		return result;
	}

	/**
	 * Get all flash cards in a named deck, filtered by category if supplied.
	*/
	std::vector<FlashCard> GetCards(std::string deck_name, std::string category = "")
	{
		std::vector<FlashCard> result;
		sqlite3* db;
		sqlite3_stmt* statement;

		constexpr std::string_view sql_raw = R"(
			SELECT
				id,
				deck_name,
				category,
				question,
				answer,
				last_answered,
				next_study_rec
			FROM flash_cards
			WHERE deck_name = '{}';
		)";
		std::string sql = std::format(sql_raw, deck_name);

		// accomodate if a category was also specified
		if (!category.empty()) {
			sql += " AND category = '" + category + "'";
		}

		sqlite3_open(db_filepath, &db);
		int err = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL);
		
		if (err != SQLITE_OK) {
			// TODO: have to hook this up to fxtui to display somewhere (message thing at bottom of page probably)
			printf("SQL Error preparing statement: %s\n", sqlite3_errmsg(db));
			sqlite3_free(statement);
			return result;
		}

		while ((err = sqlite3_step(statement)) == SQLITE_ROW) {
			FlashCard deserialized_card;
			deserialized_card.id = sqlite3_column_int(statement, 0);
			deserialized_card.deck_name = deck_name;
			deserialized_card.category = (char*)sqlite3_column_text(statement, 2);
			deserialized_card.question = (char*)sqlite3_column_text(statement, 3);
			deserialized_card.answer = (char*)sqlite3_column_text(statement, 4);
			deserialized_card.last_answered = sqlite3_column_int(statement, 5);
			deserialized_card.next_study_rec = sqlite3_column_int(statement, 6);

			result.push_back(deserialized_card);
		}

		if (err != SQLITE_DONE)
			std::cout << "Error parsing SELECT: " << sqlite3_errmsg(db) << std::endl;
		
		sqlite3_finalize(statement);
		sqlite3_close(db);

		return result;
	}

	/**
	 * Modify card with given id to match the given values of the other attibutes.
	*/
	void EditCard(FlashCard edited_card)
	{
		// TODO: fill
	}

};
