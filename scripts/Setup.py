import os
from SetupPython import PythonConfiguration as PythonRequirements

PythonRequirements.Validate()

from google_drive_downloader import GoogleDriveDownloader as gdd
from zipfile import ZipFile
from datetime import datetime

start_time = datetime.now()

zip_download_location = 'downloads/assets.zip'
temp = 'downloads/'
gdd.download_file_from_google_drive(file_id='1FVGGgka1O-eW5TIr7D-y6sc6dcCT6nM6' , dest_path=zip_download_location, unzip=False,showsize=True,overwrite=True)
file_name = zip_download_location

with ZipFile(file_name, 'r') as zip:
    zip.printdir()
  
    print('Extracting all the files now...')
    zip.extractall("ORE")
    print('Done!')

end_time = datetime.now()
print('Time Taken: {}'.format(end_time - start_time))
try:
    os.remove(zip_download_location)
    os.rmdir(temp)
    print("Directory '% s' has been removed successfully" % temp)
except OSError as error:
    print(error)
    print("Directory '% s' can not be removed" % temp)
