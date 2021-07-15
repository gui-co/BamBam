# Build

```bash
git clone https://github.com/gui-co/BamBam.git
mkdir BamBam/build
cd BamBam/build
cmake ..
make
```


# Usage

```bash
./bambam -b <bam_file> -f <fasta_file> -o <output_directory>
```

If everything goes well, this will produce the following files for each
sequence:

* `<sequence_name>.data`: start and end positions of all the transcripts
* `<sequence_name>_plus.data`: position specific data for positive strand
* `<sequence_name>_minus.data`: position specific data for negative strand

