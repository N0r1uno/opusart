# opusart
Tool to read cover images / album art from opus files.
Depends on [opusfile](https://github.com/xiph/opusfile).

## Usage
```
./opusart [type] opus_file image_file

Read opus cover art.

opus_file can be:
filename        file

image_file can be:
filename        file
    -           stdout

type can be:
0       Other
1       32x32 pixels 'file icon' (PNG only)
2       Other file icon
3       Cover (front) [default]
4       Cover (back)
5       Leaflet page
6       Media (e.g. label side of CD)
7       Lead artist/lead performer/soloist
8       Artist/performer
9       Conductor
10      Band/Orchestra
11      Composer
12      Lyricist/text writer
13      Recording Location
14      During recording
15      During performance
16      Movie/video screen capture
17      A bright colored fish
18      Illustration
19      Band/artist logotype
20      Publisher/Studio logotype
