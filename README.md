# Huffman Code Report
Huffman Code is a greedy algorithm that outputs the optimal prefix code based on frequency. We investigate Huffman Coding and its variants. Our code is in C++ and best to open the project in the IDE Clion.

## Variant 1
"In variant 1, you use straight Huffman coding, counting the number of times each symbol appears and creating a Huffman code based on this." - Spinrad

With variant 1, this is the straightforward normal Huffman Coding. We've implemented a heap with an array. We find that though the execution of various methods like min-heapify is complicated, it is very cost-effective to use an array instead. We also think it reduces the speed by quite a lot. What's also surprisingly complicated is reading in the file and modify our code to read out correctly. 

Thankfully, by relying on std::map, we limit the reading in and finding frequency at O(nlogn). This is because we walk through the entire input (n times) and each time std::map relies on red-black trees so search/insertion is (logn times). This gives us an overall of O(nlogn) to find each character's corresponding frequencies.

The building a heap and find the encoding part is O(nlogn) as the bound for Huffman Coding and we did it with the optimal Huffman Coding.

As such, we are confident we have a Huffman encoding of O(nlogn).

#### Results (Variant 1)
We used the short story <a href="http://www.galactanet.com/oneoff/theegg_mod.html">The Egg by Andy Weir</a> because it has multiple translations that we can use. For Variant 1 we only tested with the English version. We output encoding into a separate file (```encode.txt```) and the characters/frequencies in a different file (```output.txt```). We can see that we have succeeded with our encoding based on a few points. The "space" character is the most frequent, and as such, it has the shortest encoding of "110". Subsequent high-frequency encoding are the typical vowels "a", "e", "i", "o" and consonants like "t". All in all, we calculated 61 characters encoding from our program

## Variant 2
"In variant 2, you use Huffman coding on pairs of symbols rather than individual symbols." - Spinrad

This variant had us upgrade our Variant1 program to instead of using C++ char, we use C++ std::string. With a little tweaking, we add in two characters per symbol.

### Results (Variant 2)
We were expectant of this result but we still took a step back a bit after seeing it. The number of combinations exploded. We have 416 encodings this time around, and though it isn't 61x61, this can be explained by the fact that in languages, letters don't just appear next to each other completely and with regularity. We can still see some patterns that should surprise no linguistician. Inner sounds of words are higher valued while harsh consonants are less present. Of course because we did not make all characters lower-case, the upper-case encoding place them at lower values.

## Variant 3
"Variant 3 deals with the possibility that symbol frequency varies over time; for example, if you are sending text which may be books in different languages, the letter frequency changes when you switch from an English book to a German book. Therefore, you (either periodically or when you realize the frequency is off) divide the message into segments, and create optimal code for each segment." - Spinrad

Variant3 is probably the most fun the analyze but the most frustrating to code. We couldn't figure out how to implement a system based on frequency change over time so we used a brute-force of dividing up the input into chunks. Even that is major pain, as Spinrad says:

"Remember that there is a cost for these variations; you must send as well as the encoded message the encodings you are using. This is longer as you go from single to double letters, and longer if you divide the message into segments since the code must be sent for each segment."

Not just encoding, we can already see the tight extra overhead of decoding. Having to receive the encoded message, the encoding and then having to divide the message up by segments and based on what encoding do you decode. The time complexity might not be affected (once the code is set up), but the space complexity is multiplied by how many segments do we want to have. All in all, a major hassle.

### Results (Variant3)
So with this variant, we tested it using a combined German and English input. We tested out with 5 segments and 10 segments. We then compared what we have with inputs of Only English or German that were done in 5 segments. We found that the German uses about the same characters as English (Variant1), this is not surprising as they're both latin alphabets only with some slight additions on the German. What maybe surprising is that on English (Variant3), the number of characters per encoding went down to about 49, this is a reduction of about 11 characters from Variant1, which might show that there is major variation in words (and therefore characters) used at different part of a story. We do wonder what would happen if we have sourced a longer text.

For the EnglishGerman combined input, with calculation in 5 segments or 10 segments, the average number of characters have gone up, but not yet quite to 60. 5 segments is different from 10 segments because in the EnglishGerman input, with 5 segments mean the middle input has both English and German being counted in the frequency, while with 10 segments, the file is pretty much evenly divided like as if it was separate English and German. However, what we found is not too far from the expectation so we think this is a success.

## Conclusion
This was an interesting exercise though at the end we were doing more of a linguistic analysis than an algorithmic analysis. We hope that if we come back, we can implement that encoding depending on changes in frequency.