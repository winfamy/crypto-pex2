md5sum files/samplefile.txt;
for i in {0..4}
do
    md5=$(md5sum output/collision$i.txt);
    echo $md5;
done

echo '';
md5sum files/contract.txt;
md5sum output/newcontract.txt;