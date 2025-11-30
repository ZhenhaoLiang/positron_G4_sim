for i in $(seq 1 100)
  do
    export infile='out/newTPC/'$i'.root'
    export outfile='out/newTPC/'$i'.csv'
    python convert_to_csv.py --InputFile $infile --OutputFile $outfile &
 
    echo "$i"
  done
