;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2000 by Kongsberg Oil & Gas Technologies. All rights reserved.
;;; Written by mortene@sim.no, 2000-09-11.

;;; Eval following region

(define viewer (new-soxtexaminerviewer))
(-> viewer 'setscenegraph (new-socone))
(-> viewer 'show)

;;; End initial eval-region

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; To informally test SoXtExaminerViewer, play around ;;;;;;;;;;;;;;;;;;;;;

(-> viewer 'setviewing 0)
(-> viewer 'setviewing 1)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Demonstrates Bugzilla #221 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(let ((event-root (new-sogroup))
      (cube (new-socube))
      (event-node (new-SoEventCallback)))
  ; Register callback
  (-> event-node 'addEventCallback
      (SoKeyboardEvent::getClassTypeId)
      (get-scheme-event-callback-cb)
      (void-cast (callback-info (lambda (user-data event-callback)
                                  (format #t "A key was pressed~%")))))
  ; Set up scenegraph
  (-> event-root 'addchild cube)
  (-> event-root 'addchild event-node)
  ; Attach to viewer
  (-> viewer 'setscenegraph event-root)
  (-> viewer 'viewall))
