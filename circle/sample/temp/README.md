whats new?

i added my own  
                - slim screen/logger/cstring
                - slim giopin write
                - slim watchdog 
                - changed the util lfo / bpm from 2 to 1 channel ( dedicated calls )
                - unified the gfx init functions to have less specialised calls
                - also refactored and seperated the glsl / olg structs
                - changed the logger to have less calls and a whider spectrum of log outputs 

i earlier       
                - consolidated the many files into a few less - new order new names
                - included my vc04 code into the CKernel class code
                - included my parser 
                - refactored the menu
                - added a second pass menu overlay shader - still untested !!!
                - redirect confusing calls and code quirks through wrappers
                - unified as much as possible like for example the 
i still need to
                - refactor the parser that it writes into my new structs ( for the parsed data of the videos and textures )
                - im still not done with my vc04 code - i was at a point where the only issue was that i didnt manage to receive a buffertohost aka a decoded frame
                - an complete integration of all the changes, i propose a build up, start, boot-screen, allocate, load, menu ( debug ), glsl, vc04
                - consolidate the bpm/lfo working arrays/variables like before for the in/output matrix
