### About
---
What is this exactly? This is a wrapper for the [yt-dlp](https://github.com/yt-dlp/yt-dlp) cli to make it easier to work with in c++. 

It features playlist downloading, video downloading and audio downloading with it's own specific options.

### Example
---
```c++
VideoOptions options = {
  .videoOuputName = "Never gonna give you up",
  .width = 1920,
  .heigth = 1080,  
};

Video vid("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
vid.Download(options);


AudioOptions audio; // Default 
vid.DownloadAudio(audio);


PlaylistOptions playlist { 
	.isAudioOnly = true, 
	.playlistStart = 1, 
	.playlistEnd = 25 
};

Playlist play("https://www.youtube.com/playlist?list=PLuvXOFt0CoEbwWSQj5LmzPhIVKS0SvJ-1");

play.Download(playlist);
```

### Dependencies 
---
You will need to have the yt-dlp cli tool downloaded and added to your path. The download can be found [here](https://github.com/yt-dlp/yt-dlp) under releases