/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef FINGERPRINT_H
#define FINGERPRINT_H

// #include "System.h"
#include "Ap.h"
#include <mutex>
// using namespace ORB_SLAM3;



namespace ORB_SLAM3
{

class Fingerprint
{
public:
    Fingerprint():mvAp(), mvRssi(){}
    Fingerprint(const vector<Ap*> &ap, const vector<int> &rssi) :mvAp(ap), mvRssi(rssi){}
    // Copy constructor.
    Fingerprint(const Fingerprint &fp):mvAp(fp.mvAp), mvRssi(fp.mvRssi){}

    // move constructor
    Fingerprint(Fingerprint&& fp) : mvAp(std::move(fp.mvAp)), mvRssi(std::move(fp.mvRssi)) {}

    // copy assignment
    Fingerprint& operator=(const Fingerprint& rhs) {
        if (this != &rhs) { // 檢查自我賦值
            mvAp = std::move(rhs.mvAp);
            mvRssi = std::move(rhs.mvRssi);
        }
        return *this;
    }

    // move assignment
    Fingerprint& operator=(Fingerprint&& rhs) {
        if (this != &rhs) { // 檢查自我賦值
            mvAp = std::move(rhs.mvAp);
            mvRssi = std::move(rhs.mvRssi);
        }

        return *this;
    }
    
    ~Fingerprint(){}

    vector<Ap*> mvAp;
    vector<int> mvRssi;
    static std::mutex mFingerprintGlobalMutex;
    typedef boost::shared_ptr< ::ORB_SLAM3::Fingerprint> FingerprintPtr;
    typedef boost::shared_ptr< ::ORB_SLAM3::Fingerprint const> FingerprintConstPtr;
protected:
    // Mutex
    std::mutex mMutexFingerprint;
// private:
};
}
#endif // Fingerprint_H
