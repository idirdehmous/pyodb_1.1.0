import sys , os 
import re 

__all__=["OdbLexic"   ]



class OdbLexic:
    """
    Class :
    Functions and Attributes to build a given ODB Object.
    Contains the most glossary used in the ODB/SQL software 
    (Tables , names , SQL tokens etc )
    """
    def __init__(self ):
        """
        Init class ! 
        """
        return None

    def odbSqlwords(self):
        keywords=['AND'
                  'AS',
                  'BETWEEN' ,
                  'BITMAP' ,
                  'CREATE' ,
                  'DISTINCT',
                  'INDEX',
                  'ON' ,
                  'OR' ,
                  'READONLY',
                  'SELECT' ,
                  'SORT BY',
                  'SORTBY',
                  'ORDERBY' , 
                  'ORDER BY',
                  'UNIQUE' ,
                  'UPDATABLE',
                  'UPDATED',
                  'VIEW' ,
                  'WHERE',
                   '$','#','==','<','>', '||']
        return keywords 
                
    def odbFuncs(self):
        """
        THE ODB /SQL Built-in FUNCTIONS 
        USER GUIDE :https://www.ecmwf.int/sites/default/files/elibrary/2004/76278-ifs-documentation-cy36r1-part-i-observation-processing_1.pdf
                    Page No :45

        """
        funcs=[ 
                # a
               'abs','acos','acosh','acot','acot2','acoth',
               'aday','aminute','amonth','anhour','asecond',
               'asin','asinh','atan','atan2','atanh',
               'avg','avg_distinct','ayear',
                # b
               'basetime','bcount','bcount_distinct','binhi','binlo',
                # c
               'c2f','c2k','ceil','celsius','circle','cksum','cksum32','cmp',
               'conv_llu2','corr','cos','cosh','cot','coth','count','count_distinct','covar',
                # d
               'day','dble','dd','deg2rad','degrees','density','dfloat','dint','dir',
               'dist','distance','dnint','dotp','dotp_distinct','duint',
                # e
               'exp',
                # f
               'f2c','f2k','fahrenheit','ff','float','floor','fmaxval','fminval','ftrunc',
                # h
               'hour','hours',
                # i
               'ibits','int',
                # k
               'k2c','k2f','km',
                # l
               'ldexp','lg','linregr_a','linregr_b','llu2','ln','log','log10','lon0to360',
                # m
               'max','maxloc','maxval','median','median_distinct','min',
               'minloc','minute','minutes','minval','mod','month',
                # n
               'nint','norm','norm_distinct',
                # p
               'pi','pow',
                # r
               'rad','rad2deg','rms','rms_distinct',
                # s
               'second', 'seconds', 'shared2argfunc', 'sign', 'sin', 'sinh', 
               'speed' , 'sqrt', 'stdev','stdev_distinct', 'sum', 'sum_distinct',
                # t
               'tan', 'tanh', 'touch', 'trunc', 'tstamp',
                # u
               'ucom', 'ucomp', 'uint', 'uv2dd', 'uv2ff',
                # v
               'var', 'var_distinct', 'vcom', 'vcomp', 'within', 'within360',
                # y
               'year']


        return funcs

    def odbTables (self):
        tables=["desc","ddrs","hdr","body","index","poolmask","errstat","bufr","bufr_tables","bufr_tables_desc",
                    "aeolus_auxmet","aeolus_hdr","aeolus_l2c","resat","rtovs","rtovs_body","rtovs_mlev","rtovs_pred",
                    "rtovs_slev", "sat", "satem", "satob", "scatt", "scatt_body", "smos", "ralt", "ssmi", "ssmi_body", "ssmi_mlev",
                    "ssmi_slev", "timeslot_index", "update", "limb", "resat_averaging_kernel", "radar", "radar_body", "radar_station",
                    "surfbody_feedback", "modsurf", "radiance", "allsky", "co2_sink", "cloud_sink", "collocated_imager_information",
                    "auxiliary", "auxiliary_body", "radiance_body", "allsky_body", "fcdiagnostic", "gbrad", "gbrad_body", "gnssro",
                    "gnssro_body", "ensemble", "conv", "conv_body", "raingg", "raingg_body", "update_1", "update_2", "update_3", 
                    "update_4","update_5", "update_6", "update_7", "update_8", "update_9", "update_10", 
                    "enkf_1", "enkf_2", "enkf_3", "enkf_4", "enkf_5", "enkf_6", "enkf_7", "enkf_8", "enkf_9", "enkf_10", 
                    "enkf_11", "enkf_12","enkf_13", "enkf_14", "enkf_15", "enkf_16", "enkf_17", "enkf_18", "enkf_19", 
                    "enkf_20", "enkf_21", "enkf_22", "enkf_23","enkf_24", "enkf_25", "enkf_26", "enkf_27", "enkf_28", 
                    "enkf_29", "enkf_30", "enkf_31", "enkf_32", "enkf_33", "enkf_34","enkf_35", "enkf_36", "enkf_37", 
                    "enkf_38", "enkf_39", "enkf_40", "enkf_41", "enkf_42", "enkf_43", "enkf_44", "enkf_45","enkf_46", 
                    "enkf_47", "enkf_48", "enkf_49", "enkf_50", "enkf_51", "enkf_52", "enkf_53", "enkf_54", "enkf_55", 
                    "enkf_56","enkf_57", "enkf_58", "enkf_59", "enkf_60", "enkf_61", "enkf_62", "enkf_63", "enkf_64", 
                    "enkf_65", "enkf_66", "enkf_67","enkf_68", "enkf_69", "enkf_70", "enkf_71", "enkf_72", "enkf_73", 
                    "enkf_74", "enkf_75", "enkf_76", "enkf_77", "enkf_78","enkf_79", "enkf_80", "enkf_81", "enkf_82", 
                    "enkf_83", "enkf_84", "enkf_85", "enkf_86", "enkf_87", "enkf_88", "enkf_89","enkf_90", "enkf_91", 
                    "enkf_92", "enkf_93", "enkf_94", "enkf_95", "enkf_96", "enkf_97", "enkf_98", "enkf_99", "enkf_100",
                    "enkf_101", "enkf_102", "enkf_103", "enkf_104", "enkf_105", "enkf_106", "enkf_107", "enkf_108", 
                    "enkf_109", "enkf_110","enkf_111", "enkf_112", "enkf_113", "enkf_114", "enkf_115", "enkf_116", 
                    "enkf_117", "enkf_118", "enkf_119", "enkf_120",
                     "enda_1", "enda_2", "enda_3", "enda_4", "enda_5", "enda_6", "enda_7", "enda_8", "enda_9", "enda_10", 
                     "enda_11", "enda_12","enda_13", "enda_14", "enda_15", "enda_16", "enda_17", "enda_18", 
                     "enda_19", "enda_20", "enda_21", "enda_22", "enda_23","enda_24", "enda_25", "enda_26", 
                     "enda_27", "enda_28", "enda_29", "enda_30", "enda_31", "enda_32", "enda_33", "enda_34",
                     "enda_35", "enda_36", "enda_37", "enda_38", "enda_39", "enda_40", "enda_41", "enda_42", 
                     "enda_43", "enda_44", "enda_45","enda_46", "enda_47", "enda_48", "enda_49", "enda_50", 
                     "enda_51", "enda_52", "enda_53", "enda_54", "enda_55", "enda_56","enda_57", "enda_58", 
                     "enda_59", "enda_60", "enda_61", "enda_62", "enda_63", "enda_64", "enda_65", "enda_66", 
                     "enda_67","enda_68", "enda_69", "enda_70", "enda_71", "enda_72", "enda_73", "enda_74", 
                     "enda_75", "enda_76", "enda_77", "enda_78","enda_79", "enda_80", "enda_81", "enda_82", 
                     "enda_83", "enda_84", "enda_85", "enda_86", "enda_87", "enda_88", "enda_89","enda_90", 
                     "enda_91", "enda_92", "enda_93", "enda_94", "enda_95", "enda_96", "enda_97", "enda_98", 
                     "enda_99", "enda_100",
                     "surfbody_feedback_1", "surfbody_feedback_2", "surfbody_feedback_3", "surfbody_feedback_4", "surfbody_feedback_5",
                     "surfbody_feedback_6", "surfbody_feedback_7", "surfbody_feedback_8", "surfbody_feedback_9", "surfbody_feedback_10",
                     "surfbody_feedback_11", "surfbody_feedback_12", "surfbody_feedback_13", "surfbody_feedback_14", 
                     "surfbody_feedback_15","surfbody_feedback_16", "surfbody_feedback_17", "surfbody_feedback_18", 
                     "surfbody_feedback_19", "surfbody_feedback_20","surfbody_feedback_21", "surfbody_feedback_22", 
                     "surfbody_feedback_23", "surfbody_feedback_24", "surfbody_feedback_25","surfbody_feedback_26", 
                     "surfbody_feedback_27", "surfbody_feedback_28", "surfbody_feedback_29", "surfbody_feedback_30",
                     "surfbody_feedback_31", "surfbody_feedback_32", "surfbody_feedback_33", "surfbody_feedback_34", 
                     "surfbody_feedback_35","surfbody_feedback_36", "surfbody_feedback_37", "surfbody_feedback_38", 
                     "surfbody_feedback_39", "surfbody_feedback_40","surfbody_feedback_41", "surfbody_feedback_42", 
                     "surfbody_feedback_43", "surfbody_feedback_44", "surfbody_feedback_45","surfbody_feedback_46", 
                     "surfbody_feedback_47", "surfbody_feedback_48", "surfbody_feedback_49", "surfbody_feedback_50",
                     "surfbody_feedback_51", "surfbody_feedback_52", "surfbody_feedback_53", "surfbody_feedback_54", 
                     "surfbody_feedback_55","surfbody_feedback_56", "surfbody_feedback_57", "surfbody_feedback_58", 
                     "surfbody_feedback_59", "surfbody_feedback_60","surfbody_feedback_61", "surfbody_feedback_62", 
                     "surfbody_feedback_63", "surfbody_feedback_64", "surfbody_feedback_65",
                     "surfbody_feedback_66", "surfbody_feedback_67", "surfbody_feedback_68", "surfbody_feedback_69", 
                     "surfbody_feedback_70","surfbody_feedback_71", "surfbody_feedback_72", "surfbody_feedback_73", 
                     "surfbody_feedback_74", "surfbody_feedback_75", "surfbody_feedback_76", "surfbody_feedback_77", 
                     "surfbody_feedback_78", "surfbody_feedback_79", "surfbody_feedback_80","surfbody_feedback_81", 
                     "surfbody_feedback_82", "surfbody_feedback_83", "surfbody_feedback_84", "surfbody_feedback_85",
                     "surfbody_feedback_86", "surfbody_feedback_87", "surfbody_feedback_88", "surfbody_feedback_89", 
                     "surfbody_feedback_90","surfbody_feedback_91", "surfbody_feedback_92", "surfbody_feedback_93", 
                     "surfbody_feedback_94", "surfbody_feedback_95","surfbody_feedback_96", "surfbody_feedback_97", 
                     "surfbody_feedback_98", "surfbody_feedback_99", "surfbody_feedback_100",
                     "fcdiagnostic_body_1", "fcdiagnostic_body_2", "fcdiagnostic_body_3","fcdiagnostic_body_4", 
                     "fcdiagnostic_body_5", "fcdiagnostic_body_6", "fcdiagnostic_body_7","fcdiagnostic_body_8", 
                     "fcdiagnostic_body_9", "fcdiagnostic_body_10","fcdiagnostic_body_11","fcdiagnostic_body_12", 
                     "fcdiagnostic_body_13","fcdiagnostic_body_14","fcdiagnostic_body_15","fcdiagnostic_body_16", 
                     "fcdiagnostic_body_17","fcdiagnostic_body_18","fcdiagnostic_body_19","fcdiagnostic_body_20"]
        return tables 

    def odbCols (self):

        dict_cols={"desc":["expver","class","stream","type",
                           "andate","antime","inidate","initime",
                           "creadate","creatime","creaby","moddate",
                           "modtime","modby","mxup_traj","numtsl",
                           "poolmask.offset","poolmask.len",
                           "timeslot_index.offset","timeslot_index.len",
                           "fcdiagnostic.offset","fcdiagnostic.len","enda_member"] ,
                   "hdr":["seqno","subseqno","reportno", "bufrtype", "subtype", "subsetno", "groupid", "reportype",
                          "obstype", "codetype", "sensor", "retrtype", "instrument_type", "stalt", "date", "time", "distribtype",
                           "distribid", "gp_dist", "gp_number", "numlev", "numactiveb", "checksum", "sortbox", "areatype", 
                           "report_status", "report_event1","report_rdbflag", "report_blacklist", "report_event2", 
                           "thinningkey_1", "thinningkey_2", "thinningkey_3", "thinningtimekey",
                           "sitedep", "source", "lat", "lon", "trlat", "trlon", 
                           "modsurf.offset", "modsurf.len", "statid", "conv.offset", "conv.len","sat.offset", "sat.len", 
                           "body.offset", "body.len", "errstat.offset", "errstat.len", "update_1.offset", 
                           "update_1.len","update_2.offset", "update_2.len", "update_3.offset", "update_3.len", 
                           "update_4.offset", "update_4.len", "update_5.offset","update_5.len", "update_6.offset", 
                           "update_6.len", "update_7.offset", "update_7.len", "update_8.offset", "update_8.len","update_9.offset", 
                           "update_9.len", "update_10.offset", "update_10.len", "auxiliary.offset", "auxiliary.len", "ensemble.offset",
                           "ensemble.len", "gbrad.offset", "gbrad.len", "raingg.offset", "raingg.len", 
                           "surfbody_feedback.offset", "surfbody_feedback.len","window_offset", 
                           "hdr.offset", "hdr.len", "hdr_count", "aeolus_hdr.offset", "aeolus_hdr.len"]   , 


                    "conv":["linkoffset_t:conv.offset","linklen_t:conv.len",
                            "flight_phase","flight_dp_o_dt",
                            "anemoht","baroht","station_type",
                            "sonde_type","collection_identifier","country",
                            "unique_identifier","timeseries_index","conv_body.offset","ppcode",
                            "Bitfield:level","pk1int:datum_qcflag","pk1int:iternoconv_1dv"] ,
    
                    "conv_body":["conv_body.offset",
                                 "conv_body.len",
                                 "ppcode" ,
                                 "level" ,
                                 "datum_qcflag"] ,
                    "body":["body.offset", "body.len", "conv", "body", "entryno", "obsvalue", "varno", "vertco_type",
                            "vertco_reference_1", "vertco_reference_2", "datum_anflag", "datum_status", "datum_event1", 
                            "datum_rdbflag", "datum_blacklist","datum_event2", "varbc_ix", "biascorr", "biascorr_fg", 
                            "tbcorr", "bias_volatility", "wdeff_bcorr", "an_depar", "fg_depar","actual_depar", 
                            "actual_ndbiascorr", "qc_a", "qc_l", "qc_pge", "fc_sens_obs", "an_sens_obs", "jacobian_peak", 
                            "jacobian_peakl","jacobian_hpeak", "jacobian_hpeakl", "mf_vertco_type", "mf_log_p", 
                            "mf_stddev", "nlayer", 
                            "gnss_satellite_id", "azimuth","elevation", "refconst", "phi", "lon2", "lat2", 
                            "altitude2", "azimuth1", "elevation1", "refconst1", "phi1", "azimuth2","elevation2", "refconst2", 
                            "phi2", "azimuth3", "elevation3", "refconst3", "phi3", "azimuth4", "elevation4", "refconst4", "phi4"]
                         } # DICT TABLES 
        return dict_cols


    def odbPkind (self):
        list_ckind=["string",
                   "pk1int",
                   "pk9real",
                   "yyyymmdd", 
                   "hhmmss" ,
                   "Bitfield"]

        return list_ckind
   
    def odbIfsVar (self):
        ifs_vars={"$NMXUPD"          : 3  ,        # Maximum number of updates supported with this layout (min = 1 & max = 10)
                  "$NUMAUX"          : 9  ,        # No. of auxiliary obsvalue's per body; aux1 ==> aux[$NUMAUX]
                  "$NUMEV"           : 1  ,        # Maximum number of retained eigenvectors of obs. err. corr. matrix
                  "$NUMRAINTBDIAG"   : 14 ,        # No. of diagnostic output variables for cloud/rain assimilation,
                  "$MX_LIMB_TAN"     : 17 ,
                  "$MX_RADAR_NIV"    : 15 ,
                  "$MX_AK"           : 50 ,        # depends on jpmx_ak in module/parcma and bufr2odb_gch2.F90 and varindex_module.F90!!
                  "$MX_SATOB_WEIGHT" : 43 }

        return ifs_vars 

    def LinkPkind(  self):
        kind_dict={"string":"TXT"    ,
                   "pk1int":"INTEGER",
                   "pk9real":"REAL"  ,
                   "yyyymmdd":"INTEGER",
                   "hhmmss"  :"INTEGER",
                   "Bitfield":"INTEGER"}

        return kind_dict



    def sqlPrepInsert( self,  query=None ): 
        """
        Takes an CMA sql statement and convert it to 
        an sqlite3 statement for value insertion 
        """

        
        cpkind={   "date":"TXT"      ,
                   " time":"INTEGER"  ,
                   " statid":"REAL"   ,
                   " varno":"INTEGER" ,
                   " lat"  :"INTEGER" , 
                   " lon":"INTEGER"   ,
                   " obsvalue":"REAL" ,
                   " fg_depar":"REAL" }
    def sqliteEncode (self):
        """
        Empty method :  under development  !
        """
        return None
