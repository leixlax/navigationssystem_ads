#include "Dijkstra.h"
#include "k7scan1.h"
#include <algorithm>
#include <cctype>
#include <string>

string location[ANZAHL] = { "" };		//[0] = Departure; [1] = Arrival; [2] = Via
int pos_location[ANZAHL];
string cities_lowercase[ANZAHL] = { "" };
extern string cities[ANZAHL];

string string_to_lowercase(string str) {
	string lower_str;
	// Allocate the destination space
	lower_str.resize(str.size());
	//string lower_str = boost::algorithm::to_lower_copy(str);
	std::transform(str.begin(), str.end(), lower_str.begin(), ::tolower);
	return lower_str;
}

int main()
{
	//std::locale::global(std::locale("German_germany.UTF-8"));		//für die Ausgabe der Umlaute
	if (read_txt_file()) {
		return EXIT_FAILURE;
	}
	for (auto ii = 0; ii < ANZAHL; ii++) {
		cities_lowercase[ii] = string_to_lowercase(cities[ii]);
		//cout << cities_lowercase[ii] << endl;
	}

	for (auto kk = 0; kk < ANZAHL; kk++) {
		if (location[kk] == "") break;
		pos_location[kk] = comp_string_location(location[kk]);
	}

	for (auto ii = 0; ii < ANZAHL; ii++) {
		if (pos_location[ii] > ANZAHL) {
			if (ii == 0) cout << "String Departure konnte nicht identifiziert werden" << endl;
			if (ii == 1) cout << "String Arrival konnte nicht identifiziert werden" << endl;
			if (ii == 2) cout << "String Via konnte nicht identifiziert werden" << endl;
		}
	}

	cout << "Departure: " << location[0] << endl << "Arrival: " << location[1] << endl;
	dijkstra_input(pos_location[0], pos_location[1]);
	return EXIT_SUCCESS;
}

//Groß-/ Kleinschreibung implementieren
//Implementierung von dijkstra mit 3 Orten
