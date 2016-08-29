;;; fvwm.el --- major mode for editing fvwm resource files in Emacs

;; Copyright (C) 1996 Free Software Foundation, Inc.

;; Author: Daniel Pfeiffer
;; Keywords: languages, tools, unix

;; This file is part of GNU Emacs.

;; GNU Emacs is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; GNU Emacs is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;;; Commentary:

;;

;;; Code:

;;;###autoload
(or (rassq `fvwm-mode auto-mode-alist)
    (setq auto-mode-alist
  (cons `("/\.fvwm" . fvwm-mode)
auto-mode-alist)))

(defvar fvwm-mode-syntax-table nil
  "Syntax table in use in Fvwm-mode buffers.")

(if fvwm-mode-syntax-table
    ()
  (setq fvwm-mode-syntax-table (make-syntax-table))
  (modify-syntax-entry ?` """  fvwm-mode-syntax-table))

(defvar fvwm-font-lock-keywords
  `(("^\s *#.*" . font-lock-comment-face)
    (""[^"]*"\|`[^`]*`" . font-lock-string-face)

    "^\s *[a-z0-9_]+\|\s title\>"
    ("^\s *\*[a-z0-9_]+" . font-lock-function-name-face)
    ("^\s *\+\s *"[^"]*"\s *\([-a-z0-9_]+\)"
     1 font-lock-keyword-face t)
    ("^\s *\(mouse\|key\)\s +[-a-z0-9_]+\s +[-a-z0-9_]+\s +[-a-z0-9_]+\s +\([-a-z0-9_]+\)"
     2 font-lock-keyword-face t)
    ;; indeed typeface :-)
    ("\s \(-[-a-z0-9_*]+-[a-z0-9*]+\)\(\s \|$\)" 1 font-lock-type-face))
  "Additional expressions to highlight in Fvwm mode.")

;;;###autoload
(defun fvwm-mode ()
  "Major mode for editing Fvwm code.
\<fvwm-mode-map>"
  (interactive)
  (kill-all-local-variables)
  (setq major-mode `fvwm-mode)
  (setq mode-name "Fvwm")
  (set-syntax-table fvwm-mode-syntax-table)
  (make-local-variable `parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments t)
  (make-local-variable `comment-start)
  (setq comment-start "#")
  (make-local-variable `comment-start-skip)
  (setq comment-start-skip "^\s-*#\s-*")
  ;; Font lock support
  (make-local-variable `font-lock-defaults)
  (setq font-lock-defaults `(fvwm-font-lock-keywords t t))
  (run-hooks `fvwm-mode-hook))

;;; fvwm.el ends here


