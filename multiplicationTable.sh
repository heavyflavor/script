seq 9 | sed 'H;g' | awk -v RS='' '{for(i=1;i<=NF;i++) printf("%dx%d=%d%s",i,NR,i*NR,i==NR?"\n":"\t")}'
