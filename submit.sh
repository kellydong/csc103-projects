#!/bin/bash

# TODO: make sure you edit postdata to reflect your information.
. ./postdata
if (( $# != 1 )) ; then
	echo "Usage: $0 <filename>"
	exit 1
fi

rcpt=$(basename $1)-rcpt.html

curl -k --form "file=@$1" --form lname=$lastname --form sid=$sidlast4 \
	"https://www-cs.ccny.cuny.edu/~wes/CSC103/upload_file.php" > $rcpt

if (( $? != 0 )) ; then
	echo -e "\033[31mError:\033[0m upload failed.  Check network?"
	exit 1
fi
if grep -qi 'path not found' $rcpt ; then
	echo -e "\033[31mError:\033[0m Upload path not found."
	echo "Double check 'postdata' contents; make sure you are on the roster."
	exit 1
fi
if grep -qiE '(fail|warn)' $rcpt ; then
	echo -e "\033[1;31mWarning:\033[0m Receipt looks wrong.  Double check postdata?"
	exit 2
fi
if grep -qiE 'receipt:\s+[0-9a-f]{40}' $rcpt ; then
	echo -e "\033[32mSuccess (really!).\033[0m  See receipt in $rcpt"
	exit 0
fi
echo -e "\033[1;31mWarning:\033[0m Couldn't find hash in receipt.  Consult prof..."
exit 2
