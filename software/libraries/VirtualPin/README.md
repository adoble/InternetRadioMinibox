# VirtualPin Library

Many Arduino libraries take as a parameter (usually in the constructor) the pins that they use. For instance the TFT library is set up using:

`TFT(cs, dc, rst);`

Where cs, dc and rst are the pins used to connect the TFT screen.

But what happens if don't have enough pins and are using a pin expander chip? How can you specify this as a parameter to the library.

This library aims to partly solve this problem.
