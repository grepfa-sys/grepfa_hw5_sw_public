python ../tool/nvs_partition_gen.py generate nvs.csv nvs_part.bin 16384
python ../tool/parttool.py write_partition --partition-type=data --partition-subtype=nvs --input "nvs_part.bin"
python C:\Users\sys0011\AppData\Local\Programs\Python\Python312\Scripts\configure_esp_secure_cert.py -p COM11   --keep_ds_data_on_host   --efuse_key_id 1   --ca-cert ca.pem   --device-cert client.crt   --private-key private.key   --target_chip esp32s3   --secure_cert_type cust_flash_tlv   --configure_ds   --priv_key_algo RSA 2048
