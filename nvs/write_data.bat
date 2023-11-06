python ./tool/nvs_partition_gen.py generate nvs.csv nvs_part.bin 16384
python ./tool/parttool.py --port COM11 write_partition --partition-type=data --partition-subtype=nvs --input "nvs_part.bin"