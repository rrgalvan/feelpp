
# genLibFSI(
#   DIM 2
#   FLUID_U_ORDER 2
#   FLUID_P_ORDER 1
#   #FLUID_P_CONTINUITY Discontinuous
#   FLUID_GEO_ORDER 1
#   SOLID_DISP_ORDER 1
#   SOLID_GEO_ORDER 1
# )

feelpp_add_application( application_fsi_2d SRCS main_fsi.cpp
  LINK_LIBRARIES
  feelpp_model_fsi_2dP2P1G1_2dP1G1
  #feelpp_model_fsi_2dP1P1G1_2dP1G1
  DEFS FEELPP_DIM=2 FEELPP_GEO_ORDER=1
  )

feelpp_add_application( application_fsi_3d SRCS main_fsi.cpp
  LINK_LIBRARIES
  feelpp_model_fsi_3dP2P1G1_3dP1G1
  DEFS FEELPP_DIM=3 FEELPP_GEO_ORDER=1
  )

feelpp_add_application( application_fsi_3d_g2 SRCS main_fsi.cpp
  LINK_LIBRARIES
  feelpp_model_fsi_3dP2P1G2_3dP2G2
  DEFS FEELPP_DIM=3 FEELPP_GEO_ORDER=2
  )


install(TARGETS feelpp_application_fsi_2d feelpp_application_fsi_3d 
        RUNTIME DESTINATION bin/ 
        COMPONENT ModelApplications )
