; Auto-generated. Do not edit!


(cl:in-package wifi_scan-msg)


;//! \htmlinclude Fingerprint.msg.html

(cl:defclass <Fingerprint> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (seq
    :reader seq
    :initarg :seq
    :type cl:integer
    :initform 0)
   (stamp
    :reader stamp
    :initarg :stamp
    :type cl:real
    :initform 0)
   (list
    :reader list
    :initarg :list
    :type (cl:vector wifi_scan-msg:AddressRSSI)
   :initform (cl:make-array 0 :element-type 'wifi_scan-msg:AddressRSSI :initial-element (cl:make-instance 'wifi_scan-msg:AddressRSSI))))
)

(cl:defclass Fingerprint (<Fingerprint>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Fingerprint>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Fingerprint)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name wifi_scan-msg:<Fingerprint> is deprecated: use wifi_scan-msg:Fingerprint instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <Fingerprint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader wifi_scan-msg:header-val is deprecated.  Use wifi_scan-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'seq-val :lambda-list '(m))
(cl:defmethod seq-val ((m <Fingerprint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader wifi_scan-msg:seq-val is deprecated.  Use wifi_scan-msg:seq instead.")
  (seq m))

(cl:ensure-generic-function 'stamp-val :lambda-list '(m))
(cl:defmethod stamp-val ((m <Fingerprint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader wifi_scan-msg:stamp-val is deprecated.  Use wifi_scan-msg:stamp instead.")
  (stamp m))

(cl:ensure-generic-function 'list-val :lambda-list '(m))
(cl:defmethod list-val ((m <Fingerprint>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader wifi_scan-msg:list-val is deprecated.  Use wifi_scan-msg:list instead.")
  (list m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Fingerprint>) ostream)
  "Serializes a message object of type '<Fingerprint>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'seq)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'seq)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'seq)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'seq)) ostream)
  (cl:let ((__sec (cl:floor (cl:slot-value msg 'stamp)))
        (__nsec (cl:round (cl:* 1e9 (cl:- (cl:slot-value msg 'stamp) (cl:floor (cl:slot-value msg 'stamp)))))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 0) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __nsec) ostream))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'list))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'list))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Fingerprint>) istream)
  "Deserializes a message object of type '<Fingerprint>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'seq)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'seq)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'seq)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'seq)) (cl:read-byte istream))
    (cl:let ((__sec 0) (__nsec 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 0) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __nsec) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'stamp) (cl:+ (cl:coerce __sec 'cl:double-float) (cl:/ __nsec 1e9))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'list) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'list)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'wifi_scan-msg:AddressRSSI))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Fingerprint>)))
  "Returns string type for a message object of type '<Fingerprint>"
  "wifi_scan/Fingerprint")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Fingerprint)))
  "Returns string type for a message object of type 'Fingerprint"
  "wifi_scan/Fingerprint")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Fingerprint>)))
  "Returns md5sum for a message object of type '<Fingerprint>"
  "33488decb1b870040184785d23a0ab46")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Fingerprint)))
  "Returns md5sum for a message object of type 'Fingerprint"
  "33488decb1b870040184785d23a0ab46")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Fingerprint>)))
  "Returns full string definition for message of type '<Fingerprint>"
  (cl:format cl:nil "Header header~%~%uint32 seq~%time stamp~%AddressRSSI[] list~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: wifi_scan/AddressRSSI~%string address~%float64 rssi~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Fingerprint)))
  "Returns full string definition for message of type 'Fingerprint"
  (cl:format cl:nil "Header header~%~%uint32 seq~%time stamp~%AddressRSSI[] list~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: wifi_scan/AddressRSSI~%string address~%float64 rssi~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Fingerprint>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4
     8
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'list) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Fingerprint>))
  "Converts a ROS message object to a list"
  (cl:list 'Fingerprint
    (cl:cons ':header (header msg))
    (cl:cons ':seq (seq msg))
    (cl:cons ':stamp (stamp msg))
    (cl:cons ':list (list msg))
))
