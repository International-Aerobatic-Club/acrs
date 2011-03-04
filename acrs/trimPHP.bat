sed -e "s/?><?php//" %1 >sedTemp
mv %1 %1.orig
mv sedTemp %1
