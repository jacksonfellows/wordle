(defun read-lines (filename)
  (with-open-file (in filename)
    (loop for line = (read-line in nil)
          while line collect line)))

(defvar *five-letter-words* (read-lines "five-letter-words"))

(defun write-lines (words filename)
  (with-open-file (out filename :direction :output :if-exists :supersede)
    (loop for word in words
          do (write-line word out))))

(defun save-words ()
  (write-lines *five-letter-words* "five-letter-words"))

(defun pick-random-word (words)
  (elt words (random (length words))))

(defun display-diff (word guess)
  (loop for char across guess and i from 0
        do (cond
             ((char-equal char (elt word i)) (write-char #\^))
             ((find char word) (write-char #\?))
             (t (write-char #\x)))))

(defun resolve-guess (word guess last-guess)
  (cond
    ((string-equal "+" guess)
     (when last-guess
       (push last-guess *five-letter-words*)
       (format t "added ~a to the dictionary~%~a~%" last-guess last-guess)
       (save-words)
       (resolve-guess word last-guess last-guess)))
    ((string-equal "q" guess)
     (format t "the word was ~a" word))
    ((not (= 5 (length guess))) (progn
                                  (format t "not a five-letter word")
                                  (play-word word guess)))
    ((not (member guess *five-letter-words* :test #'string-equal))
     (progn
       (format t "not in the dictionary")
       (play-word word guess)))
    ((string-equal word guess) (format t "you got it!"))
    (t (progn
         (display-diff word guess)
         (play-word word guess)))))

(defun get-guess ()
  (let ((guess (read-line)))
    (if (string-equal "" guess) (get-guess) guess)))

(defun play-word (word &optional (last-guess nil))
  (resolve-guess word (get-guess) last-guess))

(defvar *good-five-letter-words* (read-lines "good-five-letter-words"))

(defun check-word-lists ()
  (loop for word in *good-five-letter-words*
        when (not (member word *five-letter-words*))
          do (format t "adding ~a to the dictionary~%" word)
             (push word *five-letter-words*)
             (save-words)))

(defun play ()
  (play-word (pick-random-word *good-five-letter-words*)))
