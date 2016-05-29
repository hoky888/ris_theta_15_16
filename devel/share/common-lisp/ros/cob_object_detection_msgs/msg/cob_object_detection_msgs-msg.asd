
(cl:in-package :asdf)

(defsystem "cob_object_detection_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :actionlib_msgs-msg
               :geometry_msgs-msg
               :sensor_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "TrainObjectFeedback" :depends-on ("_package_TrainObjectFeedback"))
    (:file "_package_TrainObjectFeedback" :depends-on ("_package"))
    (:file "TrainObjectActionGoal" :depends-on ("_package_TrainObjectActionGoal"))
    (:file "_package_TrainObjectActionGoal" :depends-on ("_package"))
    (:file "AcquireObjectImageFeedback" :depends-on ("_package_AcquireObjectImageFeedback"))
    (:file "_package_AcquireObjectImageFeedback" :depends-on ("_package"))
    (:file "AcquireObjectImageActionGoal" :depends-on ("_package_AcquireObjectImageActionGoal"))
    (:file "_package_AcquireObjectImageActionGoal" :depends-on ("_package"))
    (:file "AcquireObjectImageGoal" :depends-on ("_package_AcquireObjectImageGoal"))
    (:file "_package_AcquireObjectImageGoal" :depends-on ("_package"))
    (:file "AcquireObjectImageActionFeedback" :depends-on ("_package_AcquireObjectImageActionFeedback"))
    (:file "_package_AcquireObjectImageActionFeedback" :depends-on ("_package"))
    (:file "DetectObjectsFeedback" :depends-on ("_package_DetectObjectsFeedback"))
    (:file "_package_DetectObjectsFeedback" :depends-on ("_package"))
    (:file "TrainObjectGoal" :depends-on ("_package_TrainObjectGoal"))
    (:file "_package_TrainObjectGoal" :depends-on ("_package"))
    (:file "DetectObjectsActionResult" :depends-on ("_package_DetectObjectsActionResult"))
    (:file "_package_DetectObjectsActionResult" :depends-on ("_package"))
    (:file "AcquireObjectImageAction" :depends-on ("_package_AcquireObjectImageAction"))
    (:file "_package_AcquireObjectImageAction" :depends-on ("_package"))
    (:file "DetectObjectsActionGoal" :depends-on ("_package_DetectObjectsActionGoal"))
    (:file "_package_DetectObjectsActionGoal" :depends-on ("_package"))
    (:file "TrainObjectAction" :depends-on ("_package_TrainObjectAction"))
    (:file "_package_TrainObjectAction" :depends-on ("_package"))
    (:file "AcquireObjectImageResult" :depends-on ("_package_AcquireObjectImageResult"))
    (:file "_package_AcquireObjectImageResult" :depends-on ("_package"))
    (:file "DetectObjectsAction" :depends-on ("_package_DetectObjectsAction"))
    (:file "_package_DetectObjectsAction" :depends-on ("_package"))
    (:file "DetectObjectsActionFeedback" :depends-on ("_package_DetectObjectsActionFeedback"))
    (:file "_package_DetectObjectsActionFeedback" :depends-on ("_package"))
    (:file "TrainObjectResult" :depends-on ("_package_TrainObjectResult"))
    (:file "_package_TrainObjectResult" :depends-on ("_package"))
    (:file "TrainObjectActionResult" :depends-on ("_package_TrainObjectActionResult"))
    (:file "_package_TrainObjectActionResult" :depends-on ("_package"))
    (:file "DetectObjectsResult" :depends-on ("_package_DetectObjectsResult"))
    (:file "_package_DetectObjectsResult" :depends-on ("_package"))
    (:file "DetectObjectsGoal" :depends-on ("_package_DetectObjectsGoal"))
    (:file "_package_DetectObjectsGoal" :depends-on ("_package"))
    (:file "TrainObjectActionFeedback" :depends-on ("_package_TrainObjectActionFeedback"))
    (:file "_package_TrainObjectActionFeedback" :depends-on ("_package"))
    (:file "AcquireObjectImageActionResult" :depends-on ("_package_AcquireObjectImageActionResult"))
    (:file "_package_AcquireObjectImageActionResult" :depends-on ("_package"))
    (:file "Mask" :depends-on ("_package_Mask"))
    (:file "_package_Mask" :depends-on ("_package"))
    (:file "RectArray" :depends-on ("_package_RectArray"))
    (:file "_package_RectArray" :depends-on ("_package"))
    (:file "MaskArray" :depends-on ("_package_MaskArray"))
    (:file "_package_MaskArray" :depends-on ("_package"))
    (:file "Detection" :depends-on ("_package_Detection"))
    (:file "_package_Detection" :depends-on ("_package"))
    (:file "Rect" :depends-on ("_package_Rect"))
    (:file "_package_Rect" :depends-on ("_package"))
    (:file "PoseRT" :depends-on ("_package_PoseRT"))
    (:file "_package_PoseRT" :depends-on ("_package"))
    (:file "DetectionArray" :depends-on ("_package_DetectionArray"))
    (:file "_package_DetectionArray" :depends-on ("_package"))
  ))