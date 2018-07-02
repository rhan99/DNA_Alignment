# DNA_Alignment
This program, given two DNA sequences S and T, finds the best possible 
alignment for the two sequences based on the highest "alignment score". This
score depends on matches, mismatches, and gaps between letters which are given
the scores of +2, -1, and -5, respectively. In the final result string, matches
are represented by a "|", mismatches by a "*", and gaps by the string, s or t, 
which contains the character.

The algorithm uses dynamic programming and memoizes each intermediate score 
between substrings of S and T. Since DNA is composed of only four base 
nucleotides, similar sequences repeat themselves and the comparisons can be
stored in a dictionary. The function is called recursibely cutting the first
character of one string, then the first character of the other string, and 
finally the first character of both strings. At the end of each recursive call, 
each of three scores are compared and the best aligment is returned.