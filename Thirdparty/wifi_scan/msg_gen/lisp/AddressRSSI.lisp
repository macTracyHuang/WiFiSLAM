; Auto-generated. Do not edit!


(cl:in-package wifi_scan-msg)


;//! \htmlinclude AddressRSSI.msg.html

(cl:defclass <AddressRSSI> (roslisp-msg-protocol:ros-message)
  ((address
    :reader address
    :initarg :address
    :type cl:string
    :initform "")
   (rssi
    :reader rssi
    :initarg :rssi
    :type cl:float
    :initform 0.0))
)

(cl:defclass AddressRSSI (<AddressRSSI>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <AddressRSSI>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'AddressRSSI)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name wifi_scan-msg:<AddressRSSI> is deprecated: use wifi_scan-msg:AddressRSSI instead.")))

(cl:ensure-generic-function 'address-val :lambda-list '(m))
(cl:defmethod address-val ((m <AddressRSSI>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader wifi_scan-msg:address-val is deprecated.  Use wifi_scan-msg:address instead.")
  (address m))

(cl:ensure-generic-function 'rssi-val :lambda-list '(m))
(cl:defmethod rssi-val ((m <AddressRSSI>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader wifi_scan-msg:rssi-val is deprecated.  Use wifi_scan-msg:rssi instead.")
  (rssi m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <AddressRSSI>) ostream)
  "Serializes a message object of type '<AddressRSSI>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'address))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'address))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'rssi))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <AddressRSSI>) istream)
  "Deserializes a message object of type '<AddressRSSI>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'address) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'address) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'rssi) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<AddressRSSI>)))
  "Returns string type for a message object of type '<AddressRSSI>"
  "wifi_scan/AddressRSSI")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'AddressRSSI)))
  "Returns string type for a message object of type 'AddressRSSI"
  "wifi_scan/AddressRSSI")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<AddressRSSI>)))
  "Returns md5sum for a message object of type '<AddressRSSI>"
  "0370cfa607a1c3707a302aa570b15c1c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'AddressRSSI)))
  "Returns md5sum for a message object of type 'AddressRSSI"
  "0370cfa607a1c3707a302aa570b15c1c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<AddressRSSI>)))
  "Returns full string definition for message of type '<AddressRSSI>"
  (cl:format cl:nil "string address~%float64 rssi~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'AddressRSSI)))
  "Returns full string definition for message of type 'AddressRSSI"
  (cl:format cl:nil "string address~%float64 rssi~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <AddressRSSI>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'address))
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <AddressRSSI>))
  "Converts a ROS message object to a list"
  (cl:list 'AddressRSSI
    (cl:cons ':address (address msg))
    (cl:cons ':rssi (rssi msg))
))
