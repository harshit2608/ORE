from google_drive_downloader import GoogleDriveDownloader as gdd
from zipfile import ZipFile
from datetime import datetime
start_time = datetime.now()


gdd.download_file_from_google_drive(file_id='1FVGGgka1O-eW5TIr7D-y6sc6dcCT6nM6' , dest_path='downloads/assets.zip', unzip=False,showsize=True,overwrite=True)
file_name = "downloads/assets.zip"

with ZipFile(file_name, 'r') as zip:
    zip.printdir()
  
    print('Extracting all the files now...')
    zip.extractall("ORE")
    print('Done!')

end_time = datetime.now()
print('Duration: {}'.format(end_time - start_time))
