#ifndef YTDLP_CPP
#define YTDLP_CPP

#include <iostream>
#include <vector>

typedef struct {
  std::string videoOuputName = "%(uploader)s - %(title)s.%(ext)s"; 

  // Resolution 
  int width = 0; 
  int heigth = 0; 

  bool bestAudio = false; 
  bool bestVideo = false; 

  bool worstVideo = false; 
  bool worstAudio = false; 

  bool best = false; 
} VideoOptions; 

typedef struct {
  std::string format = "mp3"; 
  std::string bitrate = "320K";

  std::string outputPath = ""; 
  bool embedThumbnail = false; 
} AudioOptions;

typedef struct {
    std::string audioFormat = "mp3";
    std::string audioBitrate = "320K";
    std::string outputPath = "";
    std::string outputName = "%(playlist_index)s - %(title)s.%(ext)s";
    bool embedThumbnail = false;
    bool isAudioOnly = false;
    
    // Resolution
    int width = 0;
    int height = 0;
    
    // Quality options  
    bool bestAudio = false;
    bool bestVideo = false;
    bool worstVideo = false;
    bool worstAudio = false;
    bool best = false;
    
    // Playlist specific options
    std::string playlistItems = "";     
    int playlistStart = 0;              
    int playlistEnd = 0;               
    bool playlistReverse = false;       
    bool playlistRandom = false;        
    bool playlistNumbering = true;     
} PlaylistOptions;

class Playlist {
private:
    std::string playlistUrl = "";
    
public:
    int Download(const PlaylistOptions& options) {
        if (playlistUrl == "") return -1;
        
        std::string command = "yt-dlp --yes-playlist ";
        
        // Audio or video mode
        if (options.isAudioOnly) {
            command += "-x --audio-format " + options.audioFormat + " --audio-quality " + options.audioBitrate + " ";
        } else {
            // General quality control
            if (!options.best) {
                if (options.worstAudio || options.bestAudio || options.worstVideo || options.bestVideo) command += "-f ";
                if (options.worstVideo && !options.bestVideo) command += "worstvideo ";
                if (options.worstAudio && !options.bestAudio) command += "worstaudio ";
                if (options.bestVideo && !options.worstVideo) command += "bestvideo ";
                if (options.bestAudio && !options.worstAudio) command += "bestaudio ";
            } else command += "-f best ";
        }
        
        // Width and height for video
        if (!options.isAudioOnly) {
            std::vector<std::string> formatOptions;
            if (options.width > 0) formatOptions.push_back("width:" + std::to_string(options.width));
            if (options.height > 0) formatOptions.push_back("height:" + std::to_string(options.height));
            
            if (!formatOptions.empty()) {
                command += "--format-sort ";
                for (size_t i = 0; i < formatOptions.size(); i++) {
                    if (i > 0) command += ",";
                    command += formatOptions[i];
                }
                command += " ";
            } else command += "--merge-output-format mp4 ";
        }
        
        // Output path
        if (!options.outputPath.empty()) {
            command += "-P '" + options.outputPath + "' ";
        }
        
        // Playlist specific options
        if (!options.playlistItems.empty()) {
            command += "--playlist-items " + options.playlistItems + " ";
        }
        if (options.playlistStart > 0) {
            command += "--playlist-start " + std::to_string(options.playlistStart) + " ";
        }
        if (options.playlistEnd > 0) {
            command += "--playlist-end " + std::to_string(options.playlistEnd) + " ";
        }
        if (options.playlistReverse) {
            command += "--playlist-reverse ";
        }
        if (options.playlistRandom) {
            command += "--playlist-random ";
        }
        
        // Output naming
        command += "-o '" + options.outputName + "' ";
        
        // Thumbnail for audio
        if (options.embedThumbnail && options.isAudioOnly) {
            command += "--embed-thumbnail ";
        }
        
        command += playlistUrl;
        
        std::cout << command << "\n";
        return std::system(command.c_str());
    }
    
    Playlist(const std::string& url) {
        if (url == "") return;
        playlistUrl = url;
    }
    
    ~Playlist() {}
};

class Audio {
private: 
  std::string audioUrl = ""; 

public: 
  int Download(const AudioOptions& options) {
    if (audioUrl == "") return -1; 

    std::string command = "yt-dlp -x --audio-format " + options.format + " --audio-quality " + options.bitrate + " ";

    if (!options.outputPath.empty()) {
      command += " -P '" + options.outputPath + "' ";
    } 

    if (options.embedThumbnail) {
      command += " --embed-thumbnail ";
    }
  
    command += audioUrl;
    return std::system(command.c_str());
  }

  Audio(std::string& audio_url) {
    if (audio_url == "") return; 

    audioUrl = audio_url;
  }

  ~Audio() {}
}; 

class Video {
private:
   std::string videoUrl = ""; 

public:
  int Download(const VideoOptions& options ) {
    if (videoUrl == "") return -1; 

    std::string command = "yt-dlp "; 
    
    // General quality control 
    if (!options.best) {
      if (options.worstAudio || options.bestAudio || options.worstVideo || options.bestVideo) command += "-f ";

      if (options.worstVideo && !options.bestVideo) command += "worstvideo ";
      if (options.worstAudio && !options.bestAudio) command += "worstaudio ";

      if (options.bestVideo && !options.worstVideo) command += "bestvideo "; 
      if (options.bestAudio && !options.worstAudio) command += "bestaudio "; 
    
    } else command += "-f best ";
    

    // Width and height
    std::vector<std::string> formatOptions;
    if (options.width > 0) formatOptions.push_back("width:" + std::to_string(options.width)); 
    if (options.heigth > 0) formatOptions.push_back("height:" + std::to_string(options.heigth));

    if (!formatOptions.empty()) {
      command += "--format-sort ";
      for (size_t i = 0; i < formatOptions.size(); i++) {
        if (i > 0) command += ","; 
        command += formatOptions[i]; 
      }
      command += " ";
    } else command += "--merge-output-format mp4 "; // because if there is a custom format it will output as a webm 
    

    // Output name 
    command += "-o '" + options.videoOuputName + "' ";

    // Adding the final url 
    command += videoUrl; 
    std::cout << command << "\n";

    return std::system(command.c_str());
  }

  int DownloadAudio(const AudioOptions& options) {
    Audio audio(videoUrl); 
    return audio.Download(options);
  }

  Video(const std::string& url) {
    if (url == "") return; 

    videoUrl = url;
  }

  ~Video() {} 
};

#endif 