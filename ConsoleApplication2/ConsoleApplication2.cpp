#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct song {
	string name;
	song* next;
};

class playlist {
public:
	song* first_song;
	song* last_song;

	playlist() : first_song(0), last_song(0){}

	void add_song_end(string songs) {
		song* new_song = new song;

		new_song->name = songs;
		new_song->next = 0;

		if (first_song) {
			last_song->next = new_song;
			last_song = new_song;
		}

		else {
			first_song = new_song;
			last_song = new_song;
		}
	}

	void print_output(int ls) {
		if (first_song) {
			cout << "Playlist " << ls << ":" << endl;
			song* current = first_song;

			while (current) {
				cout << "> " << current->name << endl;
				current = current->next;
			}
		}
	}
};

string what_command(string input, string current_buffer, playlist *all_playlist, int& cursor_position) {
	stringstream ss(input);
	string command;

	ss >> command;

	if (command == "ADD") {
		int ls;
		ss >> ls;
		all_playlist[ls].add_song_end(current_buffer);
		current_buffer = "";
		cursor_position = -1;
	}

	else if (command == "SAVE") {
		int ls;
		ss >> ls;
		all_playlist[ls].print_output(ls);
	}

	else {
		char typed = input[0];
		current_buffer.insert(cursor_position + 1, 1, typed);
		cursor_position++;
	}

	return current_buffer;
}

int main() {
	int total_playlist;
	cin >> total_playlist;
	cin.ignore();

	playlist* all_playlist = new playlist[total_playlist];
	string current_buffer = "";
	string input;

	int cursor_position = -1;

	while (getline(cin, input)) {
		if (input == "") continue;
		current_buffer = what_command(input, current_buffer, all_playlist, cursor_position);
	}
}