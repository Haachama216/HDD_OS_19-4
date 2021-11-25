#pragma once

#include "Volume.h"
#include <iostream>

class Program
{
public:
	Program();
	~Program();

	void run();

private:
	Volume* Vol;
	//Check the entered pass with the 256-length string
	bool checkPassword(string pw, string dummy) {
		SHA256 sha256;
		uint8_t salt[3] = { 0x4E , 0x48 ,0x54 };

		// Check 1st XOR
		for (int i = 0; i < pw.length(); i++) {
			GUI::checkPassword(i);

			string toTEST_0 = pw;
			toTEST_0[i] = toTEST_0[i] ^ salt[0];

			// Check 2nd XOR
			for (int j = 0; j < pw.length(); j++) {
				string toTEST_1 = toTEST_0;
				toTEST_1[j] = toTEST_1[j] ^ salt[1];


				// Check 3rd XOR
				for (int u = 0; u < pw.length(); u++) {
					string toTEST_2 = toTEST_1;
					toTEST_2[u] = toTEST_2[u] ^ salt[2];


					// Hash
					uint8_t v = 0;
					while (true) {

						string toSHA256 = toTEST_2 + (char)v;
						toSHA256 = sha256(toSHA256);

						// Check Hashed string with database
						if (dummy.compare(toSHA256) == 0) return true;

						if (v == 255) break;
						else v++;
					}
				}
			}
		}
		return false;
	}
private:
	void openVolume(string str);
	void createVolume();
	
	void initializeVolume(string const& volumeFilePath);
	void closeVolume();
	
	void homeNavigate();
};

