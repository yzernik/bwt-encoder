Encode or decode the Burrows Wheeler Transform of a FASTA file.
Linear time encoding uses KS algorithm for direct suffix sorting.
Linear time decoding uses the backwards decoding.

To compile:
make build

usage:
to encode:
./encode -bwt [inputfile] [outputfile]
to decode:
./encode -ibwt [inputfile] [outputfile]