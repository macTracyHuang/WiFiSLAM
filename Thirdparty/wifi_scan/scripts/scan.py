#!/usr/bin/env python
import os
import time
import re
import rospy
from wifi_scan.msg import Fingerprint, AddressRSSI

class Collection:

    def __init__(self):
        # WiFi data collection commands
        self.cmd = 'sudo iw dev wlo1 scan'

    def collect(self):

        # User has pushed 'start'
        data = []
        print('WiFi started.')
        try:
            stream = os.popen(self.cmd)
            data = stream.read()
        except:
            print("\tinterface was busy")
            time.sleep(0.1)

        print("WiFi data collection complete. ")
        return data
    
    def extract_data(self, rawdata):
        # Build list of MAC addresses sensed in this sample (item).
        # Note: re.findall() returns list of found re's in the order of occurrence in the source string.
        adr = re.compile(r'\w\w:\w\w:\w\w:\w\w:\w\w:\w\w\(.*\)')
        addresses = adr.findall(rawdata)
        for i in range(len(addresses)):
            sa      = addresses[i]
            sc      = sa[:17]
            addresses[i] = sc
        # Build list of signal strengths.
        sig = re.compile(r'signal:.* dBm')
        sigs = sig.findall(rawdata)
        # Convert sigs from list of strings to list of ints
        for i in range(len(sigs)):
            sa      = sigs[i]
            sb      = sa.split()
            sc      = sb[1]
            sigs[i] = int(float(sc))

        # Make an entry to self.wifi_data that includes addresses and sigs
        assert len(addresses) == len(sigs)
        entry = [addresses, sigs]
        return entry


    def pub_data(self,data):
        pub = rospy.Publisher('wifi_fp', Fingerprint, queue_size=10)
        rospy.init_node('wifiScanner', anonymous=True)
        # r = rospy.Rate(10) #10hz

        msg = Fingerprint()

        for bssid, rssi in zip(data[0], data[1]):
            addrssi = AddressRSSI()
            addrssi.address = bssid
            addrssi.rssi = rssi
            msg.list.append(addrssi)
        
        msg.header.stamp = rospy.get_rostime()
        # rospy.loginfo(msg)
        pub.publish(msg)
        print("Number of AP: ", len(data[0]))
        # r.sleep()

if __name__ == "__main__":
    a = Collection()

    try:
        while True:
            rawdata = a.collect()

            if not rawdata:
                print("===== Warning: no scan result =====")
                continue
            
            data = a.extract_data(rawdata)
            a.pub_data(data)
    except KeyboardInterrupt:
        exit()