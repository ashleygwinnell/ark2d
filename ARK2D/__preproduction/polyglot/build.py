# coding=UTF-8
import sys
import os
import errno
import subprocess
import json 
import platform
import zlib
import base64
import binascii
import shutil
import csv;

class Enum(set):
	def __getattr__(self, name):
		if name in self:
			return name
		raise AttributeError

Language = Enum([
	"english_uk",
	"english_us",
	"french_fr",
	"french_ca",
	"spanish",
	"german",
	"italian",
	"portuguese",
	"russian",
	"greek",
	"turkish",
	"danish",
	"norwegian",
	"swedish",
	"dutch",
	"arabic",
	"polish",
	"japanese",
	"simplified_chinese",
	"finnish",
	"hebrew"
]);

data = {
	Language.english_uk: {},
	Language.english_us: {},
	Language.french_fr: {},
	Language.french_ca: {},
	Language.spanish: {},
	Language.german: {},
	Language.italian: {},
	Language.portuguese: {},
	Language.russian: {},
	Language.greek: {},
	Language.turkish: {},
	Language.danish: {},
	Language.norwegian: {},
	Language.swedish: {},
	Language.dutch: {},
	Language.arabic: {},
	Language.polish: {},
	Language.japanese: {},
	Language.simplified_chinese: {},
	Language.finnish: {},
	Language.hebrew: {}
};
 
if __name__ == "__main__":
	
	print("---");
	print("Starting conversion.");

	files = [];
	is_ark2d = True;
	output_file = "strings/_master.xml";
	if (len(sys.argv[1]) == 0):
		is_ark2d = True;
		files = ["polyglot-menus.csv", "polyglot-gameplay.csv", "polyglot-errors.csv"];
		output_file = "strings/_master.xml";
	else:
		is_ark2d = False;
		item = json.loads(sys.argv[1]);
		files.extend([item['source']]);
		output_file = item['output_folder'] + "/" + item['name'] + ".xml";

	
	for f in files:
		h = open(f);
		reader = csv.DictReader(h);

		for line in reader:
			_id = line.pop('STRING ID');
			desc = line.pop('DESCRIPTION');
			#en = line.pop('ENGLISH (US)');

			english_us = "";
			english_uk = "";
			french_fr = "";
			french_ca = "";
			spanish = "";
			german = "";
			italian = "";
			portuguese = "";
			russian = "";
			greek = "";
			turkish = "";
			danish = "";
			norwegian = "";
			swedish = "";
			dutch = "";
			arabic = "";
			polish = "";
			japanese = "";
			simplified_chinese = "";
			finnish = "";
			hebrew = "";
			

			for l, value in line.iteritems():
				lang = 0; 

				if l == "ENGLISH (UK)":
					english_uk = value;
					lang = Language.english_uk;
				elif l == "ENGLISH (US)":
					english_us = value;
					lang = Language.english_us;
				elif l == "FRANÇAIS (QUÉBEC)":
					french_ca = value;
					lang = Language.french_ca;
				elif l == "FRANÇAIS":
					french_fr = value;
					lang = Language.french_fr;
				elif l == "ESPAÑOL":
					spanish = value;
					lang = Language.spanish;
				elif l == "DEUTSCH":
					german = value;
					lang = Language.german;
				elif l == "ITALIANO":
					italian = value;
					lang = Language.italian;
				elif l == "PORTUGUÊS":
					portuguese = value;
					lang = Language.portuguese;
				elif l == "РУССКИЙ":
					russian = value;
					lang = Language.russian;
				elif l == "ΕΛΛΗΝΙΚΑ":
					greek = value;
					lang = Language.greek;
				elif l == "TURKISH":
					turkish = value;
					lang = Language.turkish;
				elif "DANISH" in l:
					danish = value;
					lang = Language.danish;
				elif "NORWEGIAN" in l:
					norwegian = value;
					lang = Language.norwegian;
				elif l == "SWEDISH":
					swedish = value;
					lang = Language.swedish;
				elif "DUTCH" in l:
					dutch = value;
					lang = Language.dutch;
				elif "العربية" in l:
					arabic = value;	
					lang = Language.arabic;
				elif "POLISH" in l or "POLSKI" in l:
					polish = value;	
					lang = Language.polish;
				elif l == "JAPANESE" or "JAPANESE" in l:
					japanese = value;
					lang = Language.japanese;
				elif "ZH_CN" in l or "Simplified Chinese" in l:
					simplified_chinese = value;
					lang = Language.simplified_chinese;
				elif l == "FINNISH":
					finnish = value;
					lang = Language.finnish;
				elif "Hebrew" in l:
					hebrew = value;
					lang = Language.hebrew;

				if lang != 0 and len(value):
					data[lang][_id.lower()] = value;

				#print(l + " - " + value);

				
			#continue; 
			
		h.close();
	
	# Loop through data and create the output files
	"""
	for language in data:
		lines = "";
		lines += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
		lines += "<strings>\n";
		for key in data[language]:
			lines += "	<string name=\"" + key + "\" lang=\"" + language + "\">" + data[language][key] + "</string>\n";
		pass;
		lines += "</strings>\n";
		
		f = "strings/" + language + ".xml";
		print("Writing file: " + f);
		h = open(f, "w");
		h.write(lines);
		h.close();
	"""

	# Create as "master" file. 
	lines = "";
	lines += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	lines += "<strings grouped=\"true\">\n";
	for language in data:
		lines += "	<group lang=\"" + language + "\">\n";
		for key in data[language]:
			lines += "		<string name=\"" + key + "\">" + data[language][key] + "</string>\n";
		lines += "	</group>\n";
	lines += "</strings>\n";

	
	print("Writing file: " + output_file);
	h = open(output_file, "w");
	h.write(lines);
	h.close();

	# Add support for other file types, e.g. binary, android strings xml, nsdict.
	# ... 

	# Copy it to ARK2D data directory (if it's polyglot/ark2d native).
	if (is_ark2d):
		f1 = open("../../data/strings.xml", "w");
		f1.write(lines);
		f1.close();
	
	#print(data);
	print("Done!");
	

		
