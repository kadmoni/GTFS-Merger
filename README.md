# GTFS-Merger
GTFS MERGE README:
This program is meant for merging GTFS files downloaded from Israel's Transportation Admin: https://gtfs.mot.gov.il/gtfsfiles/,
and GTFS files Extracted from Transcad.
Prerequisites:
1. Make sure you have 7zip installed on your computer. link: https://www.7-zip.org/download.html
Instructions:
1. Extract GTFS files from Transcad and place them in \New
2. Download Israel's GTFS zip from the given link and place the zip in \Current
3. Run Add new lines from Transcad.exe and follow the on-screen instructions (About 15-20 minutes usual runtime).
4. When the program finshes running there'll be outputs in two folder:
	a. All proccessed GTFS files are in \Combined 
	b. A copy of the same GTFS files are in a zipped folder in \Combined_Zip, named according to the accessbilty map's program's conventions.	

NOTES:
	1.Right now you can choose only to add lines that start and end at the same time. EVEN THOUGH YOU HAVE AN OPTION TO CHOOSE OTHERWISE,
	choosing the option of time per line will give unexpected results or crash.
	3.Loading is not dynamic yet. meaning that loading precentages are only an estimate based on the usual size of Israel's GTFS.
	2.DON'T RUN ANY OTHER FILES OTHER THAN THE ONES INSTRUCTED. other files are still in beta and might not work