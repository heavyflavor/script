#get_file_list.pl -distinct -keys "runnumber" -cond "storage=hpss,production=P11id,trgsetupname=AuAu27_production_2011,filetype=daq_reco_MuDst,filename~st_physics,tof=1" -o Run11_27GeV_P11id_runnumber.list -delim '/' -limit 0

get_file_list.pl -distinct -keys "runnumber" -cond "storage=hpss,production=P10ik,trgsetupname=AuAu39_production,filetype=daq_reco_MuDst,filename~st_physics,tof=1" -o Run10_39GeV_P10ik_runnumber.list -delim '/' -limit 0
