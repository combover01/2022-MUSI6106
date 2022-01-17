wavFile = audioread("hooch.wav");
textFile = load('outputText.txt');



diff = wavFile - textFile;
%precision difference:
maxDiff = max(abs(diff))
%so the maximum difference is 5e-7 which makes sense as converting floats to
%strings gives 6 decimal point precision.