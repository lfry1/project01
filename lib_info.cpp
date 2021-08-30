/* FILENAME: lib_info.cpp
 * AUTHORS: Louise Fry and Mikaela Atkinson     
 * DATE: 8/25/2021
 * This file sorts,formats, and prints "raw" mp3 track info      
 * with format title_time(m:ss)_artist_album_genre_track.  
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <iomanip>

using namespace std;

struct song_t {
  int track;
  string title;
  int time;
} tempSong;

struct album_t {
  string title;
  int numSongs;
  int totalTime;
  map<int, song_t> songs;
} tempAlbum;

struct artist_t {
  string name;
  int numSongs;
  int totalTime;
  map<string, album_t> albums;
} tempArtist;

int main(int argc, char **argv) {

  map<string, artist_t> artists;
  map<string, artist_t>::iterator artistIt;
  map<string, album_t>::iterator albumIt;
  map<int, song_t>::iterator songIt;
  ifstream musicFile(argv[1]);
  string line, songTitle, songTime, artistName, albumName, genre, track, min, sec;
  int m, s, time, t;

  // read file into objects/variables
  musicFile >> songTitle >> songTime >> artistName >> albumName >> genre >> track;
  while ( !musicFile.eof() ) {
    replace( songTitle.begin(), songTitle.end(), '_', ' ');
    replace( artistName.begin(), artistName.end(), '_', ' ');
    replace( albumName.begin(), albumName.end(), '_', ' ');

    size_t pos = songTime.find(':');
    min = songTime.substr( 0, pos );
    sec = songTime.substr( pos+1 );
    // convert minutes to seconds and together for total song time
    m = stoi(min)*60;
    s = stoi(sec);
    time = m + s;
    // convert track string to int
    t = stoi(track);

    // check if artist exists in map
    artistIt = artists.find( artistName );
    // if artist NOT in map add it
    if( artistIt == artists.end() ) {
      tempArtist.name = artistName;
      artists[ artistName ] = tempArtist;
    }
    // check if album already exists in map, add if not
    albumIt = artists[ artistName ].albums.find( albumName );
    // if it DOES NOT exist...
    if ( albumIt == artists[ artistName ].albums.end() ) {
      tempAlbum.title = albumName;
      artists[ artistName ].albums[ albumName ] = tempAlbum;
    }
    // add song
    tempSong.track = t;
    tempSong.title = songTitle;
    tempSong.time = time;
    artists[ artistName ].albums[ albumName ].songs[ t ] = tempSong;
    // update total time and number of songs variables in Artist and Album objects
    artists[ artistName ].albums[ albumName ].totalTime += time;
    artists[ artistName ].albums[ albumName ].numSongs += 1;
    artists[ artistName ].totalTime += time;
    artists[ artistName ].numSongs += 1;
    // read in next line here to test for eof
    musicFile >> songTitle >> songTime >> artistName >> albumName >> genre >> track;
  };

  // print map
  for( artistIt = artists.begin(); artistIt != artists.end(); artistIt++) {
    m = artistIt->second.totalTime/60;
    s = artistIt->second.totalTime - ( m*60 );
    cout << artistIt->second.name << ": " << artistIt->second.numSongs << ", " << m << ":";
    if( s < 10 ) {
      cout << 0;
    }
    cout << s << endl;
    for( albumIt = artistIt->second.albums.begin(); albumIt != artistIt->second.albums.end(); ++albumIt ) {
      m = albumIt->second.totalTime/60;
      s = albumIt->second.totalTime - ( m*60 );
      cout << "        " << albumIt->second.title << ": "<< albumIt->second.numSongs << ", " << m << ":";
      if( s < 10 ) {
        cout << 0;
      }
      cout << s << endl;
      for( songIt = albumIt->second.songs.begin(); songIt != albumIt->second.songs.end(); ++songIt ) {
        m = songIt->second.time/60;
        s = songIt->second.time - ( m*60 );
        cout << "                " << songIt->second.track << ". " << songIt->second.title
             << ": " << m << ":";
        if( s < 10 ) {
          cout << 0;
        }
        cout << s << endl;
      }
    }
  }

  return 0;
}

