for seed in {1..5}
do
    for l in {1..1024}
    do
        python relocation.py -s $seed -l $l -n 500 -a 1k -p 10m -c >> log.txt
    done
done