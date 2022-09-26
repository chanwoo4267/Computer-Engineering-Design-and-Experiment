
echo "working directory: "
read path
if [ "x${path}" == "x" ];
then 
 path="`pwd -P`"
fi

cd -- "$path" 2>/dev/null || { echo NO_DIRECTION; exit 1; }

for j in *
do 
	string=""
	len="${#j}"

	for((i=0;i<$"len"+1;i++))
	do
		temp=${j:$i:1}
		case "${temp}" in
		[a-z]) 
		temp=`echo $temp | tr '[a-z]' '[A-Z]'`
		string="${string}${temp}";;
		[A-Z])
		temp=`echo $temp | tr '[A-Z]' '[a-z]'`
		string="${string}${temp}";;
		esac
	done

	mv $j $string;

done


