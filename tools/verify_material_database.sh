XMLFILE=$1 
DTDFILE=$2
xmllint --dtdvalid $2 --noout $XMLFILE 
