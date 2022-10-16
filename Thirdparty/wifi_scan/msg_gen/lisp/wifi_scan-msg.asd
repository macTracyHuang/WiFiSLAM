
(cl:in-package :asdf)

(defsystem "wifi_scan-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "AddressRSSI" :depends-on ("_package_AddressRSSI"))
    (:file "_package_AddressRSSI" :depends-on ("_package"))
    (:file "Fingerprint" :depends-on ("_package_Fingerprint"))
    (:file "_package_Fingerprint" :depends-on ("_package"))
  ))