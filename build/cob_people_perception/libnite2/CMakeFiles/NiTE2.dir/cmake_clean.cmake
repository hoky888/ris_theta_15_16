FILE(REMOVE_RECURSE
  "/home/team_theta/ROS/devel/lib/libNiTE2.pdb"
  "/home/team_theta/ROS/devel/lib/libNiTE2.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/NiTE2.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
