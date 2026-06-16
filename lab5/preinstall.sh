#!/bin/bash
echo "=== Налаштування оточення для ЛР №5 ==="

sudo apt update
sudo apt install python3-venv python3-pip curl -y

python3 -m venv venv

source venv/bin/activate

pip install --upgrade pip
pip install -r requirements.txt

echo "=== Автоматичне завантаження файлів даних VHI ==="
mkdir -p vhi_data

for i in {1..27}
do
   echo "Завантаження даних для області ID: $i..."
   curl -s -o "vhi_data/vhi_id_${i}.csv" "https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/get_TS_page.php?country=UKR&province=${i}"
done

echo "=== Підготовка середовища успішно завершена! ==="
