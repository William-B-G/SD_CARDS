/* 

	This file defines input and output definitions

*/


#define i_DN DN[dn_limit_io_sel],DNB[dn_limit_io_sel],DND[dn_limit_io_sel]
#define i_DT DT[dn_limit_io_sel],DTB[dn_limit_io_sel],DTD[dn_limit_io_sel]
#define i_DT1 DT1[dn_limit_io_sel],DT1B[dn_limit_io_sel],DT1D[dn_limit_io_sel]
#define i_DT2 DT2[dn_limit_io_sel],DT2B[dn_limit_io_sel],DT2D[dn_limit_io_sel]
#define i_DT3 DT3[dn_limit_io_sel],DT3B[dn_limit_io_sel],DT3D[dn_limit_io_sel]
#define i_DT4 DT4[dn_limit_io_sel],DT4B[dn_limit_io_sel],DT4D[dn_limit_io_sel]
#define i_DT5 DT5[dn_limit_io_sel],DT5B[dn_limit_io_sel],DT5D[dn_limit_io_sel]
#define i_DT6 DT6[dn_limit_io_sel],DT6B[dn_limit_io_sel],DT6D[dn_limit_io_sel]
#define i_DTS DTS[dn_limit_io_sel],DTSB[dn_limit_io_sel],DTSD[dn_limit_io_sel]
#define i_UN UN[up_limit_io_sel],UNB[up_limit_io_sel],UND[up_limit_io_sel]
#define i_UT UT[up_limit_io_sel],UTB[up_limit_io_sel],UTD[up_limit_io_sel]
#define i_UT1 UT1[up_limit_io_sel],UT1B[up_limit_io_sel],UT1D[up_limit_io_sel]
#define i_UT2 UT2[up_limit_io_sel],UT2B[up_limit_io_sel],UT2D[up_limit_io_sel]
#define i_UT3 UT3[up_limit_io_sel],UT3B[up_limit_io_sel],UT3D[up_limit_io_sel]
#define i_UT4 UT4[up_limit_io_sel],UT4B[up_limit_io_sel],UT4D[up_limit_io_sel]
#define i_UT5 UT5[up_limit_io_sel],UT5B[up_limit_io_sel],UT5D[up_limit_io_sel]
#define i_UT6 UT6[up_limit_io_sel],UT6B[up_limit_io_sel],UT6D[up_limit_io_sel]
#define i_UTS UTS[up_limit_io_sel],UTSB[up_limit_io_sel],UTSD[up_limit_io_sel]
#define i_DL_1 DL_1[lev_io_sel],DL_1B[lev_io_sel],DL_1D[lev_io_sel]
#define i_DL DL[lev_io_sel],DLB[lev_io_sel],DLD[lev_io_sel]
#define i_DZ DZ[dz_io_sel],DZB[dz_io_sel],DZD[dz_io_sel]
#define i_DZA DZA[dza_io_sel],DZAB[dza_io_sel],DZAD[dza_io_sel]
#define i_UL UL[lev_io_sel],ULB[lev_io_sel],ULD[lev_io_sel]
#define i_UL_1 UL_1[lev_io_sel],UL_1B[lev_io_sel],UL_1D[lev_io_sel]
#define i_STP STP,STPB,STPD
#define i_MRIE MRIE[mrie_io_sel],MRIEB[mrie_io_sel],MRIED[mrie_io_sel]
#define i_EBKS EBKS[ebks_io_sel],EBKSB[ebks_io_sel],EBKSD[ebks_io_sel]
#define i_EBK1 EBK1i,EBK1iB,EBK1iD
#define i_EBK2 EBK2i,EBK2iB,EBK2iD
#define i_EBA EBAi,EBAiB,EBAiD
#define i_EBB EBBi,EBBiB,EBBiD
#define i_LC LC,LCB,LCD
#define i_READY READY,READYB,READYD
#define i_GTS GTS,GTSB,GTSD
#define i_BP1 BP1,BP1B,BP1D
#define i_BP2 BP2,BP2B,BP2D
#define i_BP4 BP4,BP4B,BP4D
#define i_BP8 BP8,BP8B,BP8D
#define i_BP16 BP16,BP16B,BP16D
#define i_BP32 BP32,BP32B,BP32D
#define i_UF UFI,UFIB,UFID
#define i_DF DFI,DFIB,DFID
#define i_UP UPI,UPIB,UPID
#define i_DNR DNI,DNIB,DNID	
#define i_GOV GOV,GOVB,GOVD
#define i_TPL TPL,TPLB,TPLD
#define i_P P,PB,PD
#define i_DEL DEL,DELB,DELD
#define i_DON DON,DONB,DOND
#define i_GS GS,GSB,GSD
#define i_GS_1 GS_1,GS_1B,GS_1D
#define i_RGS RGS,RGSB,RGSD
#define i_RGS_1 RGS_1,RGS_1B,RGS_1D
#define i_LWA LWA,LWAB,LWAD
#define i_BKS BKS,BKSB,BKSD
#define i_XBKS1 XBKS1,XBKS1B,XBKS1D
#define i_BKS1 BKS1,BKS1B,BKS1D 
#define i_BKS2 BKS2,BKS2B,BKS2D
#define i_BDBS BDBS,BDBSB,BDBSD
#define i_CTS CTS,CTSB,CTSD
#define i_PS PS,PSB,PSD
#define i_CSS CSS,CSSB,CSSD
#define i_SEE SEE,SEEB,SEED
#define i_TEE TEE,TEEB,TEED
#define i_HMO HMO,HMOB,HMOD
#define i_CMP CMP,CMPB,CMPD
#define i_BFS BFS,BFSB,BFSD
#define i_PDS PDS,PDSB,PDSD
#define i_BF BF,BFB,BFD
#define i_TF TF,TFB,TFD
#define i_TGDO TGDO,TGDOB,TGDOD
#define i_TGDOR TGDOR,TGDORB,TGDORD
#define i_TGDC TGDC,TGDCB,TGDCD
#define i_OVL OVL,OVLB,OVLD
#define i_CCPBS CCPBS,CCPBSB,CCPBSD
#define i_ED ED[io_sel],EDB[io_sel],EDD[io_sel]
#define i_RM_ED RM_ED[f_rm_io_sel],RM_EDB[f_rm_io_sel],RM_EDD[f_rm_io_sel]


#define i_ATTUP ATTUP[io_sel],ATTUPB[io_sel],ATTUPD[io_sel]
#define i_ATTDN ATTDN[io_sel],ATTDNB[io_sel],ATTDND[io_sel]
#define i_ATT ATT[io_sel],ATTB[io_sel],ATTD[io_sel]
#define i_ATTBY ATTBY[io_sel],ATTBYB[io_sel],ATTBYD[io_sel]
#define i_FST FSTI,FSTIB,FSTID
#define i_MCC MCCI,MCCIB,MCCID
#define i_MCA MCAI,MCAIB,MCAID
#define i_RUNA RUNAI,RUNAIB,RUNAID
#define i_RUN RUNI,RUNIB,RUNID
#define i_FFS FFS,FFSB,FFSD
#define i_SS SS[ss_io_sel],SSB[ss_io_sel],SSD[ss_io_sel]
#define i_AD AD,ADB,ADD
#define i_DLT DLT,DLTB,DLTD
#define i_DLB DLB_,DLB_B,DLB_D
#define i_DLT_1 DLT_1,DLT_1B,DLT_1D
#define i_DLB_1 DLB_1,DLB_1B,DLB_1D
#define i_DLM DLM,DLMB,DLMD
#define i_DLM_1 DLM_1,DLM_1B,DLM_1D
#define i_RLM RLM,RLMB,RLMD
#define i_RLM_1 RLM_1,RLM_1B,RLM_1D
#define i_ELOF ELOF,ELOFB,ELOFD
#define i_DZSA DZSA,DZSAB,DZSAD
#define i_UNS UNS,UNSB,UNSD
#define i_DNS DNS,DNSB,DNSD
#define i_SFC SFC,SFCB,SFCD
#define i_CWS CWS[a2004_io_sel],CWSB[a2004_io_sel],CWSD[a2004_io_sel]
#define i_HC HC,HCB,HCD
#define i_CCP CCP,CCPB,CCPD
#define i_CCLP CCLP,CCLPB,CCLPD
#define i_HCLP HCLP,HCLPB,HCLPD
#define i_LHCP LHCP,LHCPB,LHCPD
#define i_DMO DMO_,DMO_B,DMO_D
#define i_DLW DLW[dlw_io_sel],DLWB[dlw_io_sel],DLWD[dlw_io_sel]
#define i_GBP GBP,GBPB,GBPD
#define i_LBP LBP,LBPB,LBPD
#define i_ATTHC ATTHC[io_sel],ATTHCB[io_sel],ATTHCD[io_sel]
#define i_SAB SAB,SABB,SABD
#define i_SECFM SECFM,SECFMB,SECFMD
#define i_HBE HBE[io_sel],HBEB[io_sel],HBED[io_sel]
#define i_OSERA OSERA[0],OSERAB[0],OSERAD[0]
#define i_INS INS,INSB,INSD
#define i_IU IU,IUB,IUD
#define i_ID ID,IDB,IDD
#define i_INE INE,INEB,INED
#define i_ACC ACC,ACCB,ACCD
#define i_IND IND,INDB,INDD
#define i_INDC INDC[io_sel],INDCB[io_sel],INDCD[io_sel]
#define i_RM_INDC RM_INDC[f_rm_io_sel],RM_INDCB[f_rm_io_sel],RM_INDCD[f_rm_io_sel] 
#define i_INDS INDS,INDSB,INDSD
#define i_IDSO IDSO[io_sel],IDSOB[io_sel],IDSOD[io_sel]
#define i_SCS SCS[f_rm_io_sel],SCSB[f_rm_io_sel],SCSD[f_rm_io_sel]
#define i_SCSR SCSR[r_rm_io_sel],SCSRB[r_rm_io_sel],SCSRD[r_rm_io_sel]
#define i_THDS THDS,THDSB,THDSD
#define i_DOB DOBI[io_sel],DOBIB[io_sel],DOBID[io_sel]
#define i_FiDOB FiDOB,FiDOBB,FiDOBD
#define i_DOBR DOBR[rcc_io_sel],DOBRB[rcc_io_sel],DOBRD[rcc_io_sel]
#define i_FiDOBR FiDOBR,FiDOBRB,FiDOBRD
#define i_GRT2 GRT2,GRT2B,GRT2D
#define i_GRT1 GRT1,GRT1B,GRT1D
#define i_BRK BRKI,BRKIB,BRKID
#define i_EBK EBKi,EBKiB,EBKiD
#define i_DS DS,DSB,DSD
#define i_US US,USB,USD
#define i_DCB DCB[io_sel],DCBIB[io_sel],DCBID[io_sel]
#define i_FiDCB FiDCB,FiDCBB,FiDCBD
#define i_DCBR DCBR[rcc_io_sel],DCBRB[rcc_io_sel],DCBRD[rcc_io_sel]
#define i_FiDCBR FiDCBR,FiDCBRB,FiDCBRD
#define i_AUTO AUTO,AUTOB,AUTOD
#define i_MRIN MRIN,MRINB,MRIND
#define i_MRIU MRIU,MRIUB,MRIUD
#define i_MRID MRID,MRIDB,MRIDD
#define i_ICI ICI,ICIB,ICID
#define i_ICID ICID_,ICID_B,ICID_D
#define i_ICIU ICIU,ICIUB,ICIUD
#define i_TAU TAU,TAUB,TAUD
#define i_TAD TAD,TADB,TADD
#define i_BAU BAU,BAUB,BAUD
#define i_BAD BAD,BADB,BADD
#define i_EQR EQR[eq_io_sel],EQRB[eq_io_sel],EQRD[eq_io_sel]
#define i_ETH ETH[eq_io_sel],ETHB[eq_io_sel],ETHD[eq_io_sel]
#define i_COL COL[eq_io_sel],COLB[eq_io_sel],COLD[eq_io_sel]
#define i_LWB LWB,LWBB,LWBD
#define i_LWD LWD,LWDB,LWDD
#define i_ZON ZON[io_sel],ZONB[io_sel],ZOND[io_sel]

#define i_CC(fl) cc[io_sel][fl],ccb[io_sel][fl],ccd[io_sel][fl]
#define o_CCL(fl) ccl[io_sel][fl],cclb[io_sel][fl],ccld[io_sel][fl]
#define i_CCR(fl) ccr[rcc_io_sel][fl],ccrb[rcc_io_sel][fl],ccrd[rcc_io_sel][fl]
#define o_CCRL(fl) ccrl[rcc_io_sel][fl],ccrlb[rcc_io_sel][fl],ccrld[rcc_io_sel][fl]
#define i_UC(fl) uc[fl],ucb[fl],ucd[fl]
#define o_UCL(fl) ucl[fl],uclb[fl],ucld[fl]
#define i_DC(fl) dc[fl],dcb[fl],dcd[fl]
#define o_DCL(fl) dcl[fl],dclb[fl],dcld[fl]
#define i_CCS(fl) ccs[fl],ccsb[fl],ccsd[fl]
#define i_RCCS(fl)  rccs[fl],rccsb[fl],rccsd[fl]
#define i_RM_CC(fl) rm_cc[f_rm_io_sel][fl],rm_ccb[f_rm_io_sel][fl],rm_ccd[f_rm_io_sel][fl]
#define o_RM_CCL(fl) rm_ccl[f_rm_io_sel][fl],rm_cclb[f_rm_io_sel][fl],rm_ccld[f_rm_io_sel][fl]
#define i_RM_CCR(fl) rm_ccr[r_rm_io_sel][fl],rm_ccrb[r_rm_io_sel][fl],rm_ccrd[r_rm_io_sel][fl]
#define o_RM_CCRL(fl) rm_ccrl[r_rm_io_sel][fl],rm_ccrlb[r_rm_io_sel][fl],rm_ccrld[r_rm_io_sel][fl]
#define o_SCCL(fl) sccl[fl],scclb[fl],sccld[fl]
#define o_SCCRL(fl) sccrl[fl],sccrlb[fl],sccrld[fl]


#define i_FS FS[hfire_io_sel],FSB[hfire_io_sel],FSD[hfire_io_sel]
#define i_BP BP[hfire_io_sel],BPB[hfire_io_sel],BPD[hfire_io_sel]
#define i_FSX FSX[hfire_io_sel],FSXB[hfire_io_sel],FSXD[hfire_io_sel]
#define i_ALT ALT[hfire_io_sel],ALTB[hfire_io_sel],ALTD[hfire_io_sel] 
#define i_MES MES[hfire_io_sel],MESB[hfire_io_sel],MESD[hfire_io_sel]
#define i_MRS MRS[hfire_io_sel],MRSB[hfire_io_sel],MRSD[hfire_io_sel]
#define i_HWS HWS[hfire_io_sel],HWSB[hfire_io_sel],HWSD[hfire_io_sel]
#define i_HWS2 HWS2[hfire_io_sel],HWS2B[hfire_io_sel],HWS2D[hfire_io_sel]
#define i_FS2 FS2[io_sel],FS2B[io_sel],FS2D[io_sel]
#define i_FS2H FS2H[io_sel],FS2HB[io_sel],FS2HD[io_sel]
#define i_FS2C FS2C[io_sel],FS2CB[io_sel],FS2CD[io_sel]
#define i_FiFS2C FiFS2C,FiFS2CB,FiFS2CD
#define i_EMP EMP[hemp_io_sel],EMPB[hemp_io_sel],EMPD[hemp_io_sel]
#define i_EPS EPS[hemp_io_sel],EPSB[hemp_io_sel],EPSD[hemp_io_sel]
#define	i_EPT EPT[ept_io_sel],EPTB[ept_io_sel],EPTD[ept_io_sel]
#define i_HEOF HEOF[heof_io_sel],HEOFB[heof_io_sel],HEOFD[heof_io_sel]
#define i_EOR EOR[eor_io_sel],EORB[eor_io_sel],EORD[eor_io_sel]
#define i_JAILB JAILB[heof_io_sel],JAILBB[heof_io_sel],JAILBD[heof_io_sel]
#define i_HUGI HUGI[heof_io_sel],HUGIB[heof_io_sel],HUGID[heof_io_sel]
#define i_HUGB HUGB[io_sel],HUGBB[io_sel],HUGBD[io_sel]
#define i_RTL RTL[rtl_io_sel],RTLB[rtl_io_sel],RTLD[rtl_io_sel]
#define i_RTLH RTLH[rtlh_io_sel],RTLHB[rtlh_io_sel],RTLHD[rtlh_io_sel] 
#define i_RTLDC RTLDC[rtldc_io_sel],RTLDCB[rtldc_io_sel],RTLDCD[rtldc_io_sel]
#define i_EMSH EMSH[emsh_io_sel],EMSHB[emsh_io_sel],EMSHD[emsh_io_sel]
#define i_SE SE,SEB,SED
#define i_EE EE[ee_io_sel],EEB[ee_io_sel],EED[ee_io_sel]
#define i_DCL DCL[fdoor_io_sel],DCLB[fdoor_io_sel],DCLD[fdoor_io_sel]
#define i_DOL DOL[fdoor_io_sel],DOLB[fdoor_io_sel],DOLD[fdoor_io_sel]
#define i_DPM DPM[fdoor_io_sel],DPMB[fdoor_io_sel],DPMD[fdoor_io_sel]
#define i_SER SER[rdoor_io_sel],SERB[rdoor_io_sel],SERD[rdoor_io_sel]
#define i_EER EER[eer_io_sel],EERB[eer_io_sel],EERD[eer_io_sel]
#define i_DCLR DCLR[rdoor_io_sel],DCLRB[rdoor_io_sel],DCLRD[rdoor_io_sel]
#define i_DOLR DOLR[rdoor_io_sel],DOLRB[rdoor_io_sel],DOLRD[rdoor_io_sel]
#define i_RPM RPM[rdoor_io_sel],RPMB[rdoor_io_sel],RPMD[rdoor_io_sel]
#define i_ALRM ALRM[alrm_io_sel],ALRMB[alrm_io_sel],ALRMD[alrm_io_sel]
#define i_EMS EMS[io_sel],EMSB[io_sel],EMSD[io_sel]
#define i_LPS LPS[lps_io_sel],LPSB[lps_io_sel],LPSD[lps_io_sel]
#define i_LOS LOS[los_io_sel],LOSB[los_io_sel],LOSD[los_io_sel]
#define i_BDC BDC[bdc_io_sel],BDCB[bdc_io_sel],BDCD[bdc_io_sel]
#define i_TDC TDC[tdc_io_sel],TDCB[tdc_io_sel],TDCD[tdc_io_sel]
#define i_MDC MDC[mdc_io_sel],MDCB[mdc_io_sel],MDCD[mdc_io_sel]
#define i_MDCR MDCR[mdc_io_sel],MDCRB[mdc_io_sel],MDCRD[mdc_io_sel]
#define i_RM_DOB RM_DOBI[f_rm_io_sel],RM_DOBIB[f_rm_io_sel],RM_DOBID[f_rm_io_sel]
#define i_RM_DCB RM_DCBI[f_rm_io_sel],RM_DCBIB[f_rm_io_sel],RM_DCBID[f_rm_io_sel]
#define i_RM_DOBR RM_DOBR[r_rm_io_sel],RM_DOBRB[r_rm_io_sel],RM_DOBRD[r_rm_io_sel]
#define i_RM_DCBR RM_DCBR[r_rm_io_sel],RM_DCBRB[r_rm_io_sel],RM_DCBRD[r_rm_io_sel]
#define i_TPH TPH,TPHB,TPHD


#define i_std_EE EE[0],EEB[0],EED[0]
#define i_std_DCL DCL[0],DCLB[0],DCLD[0]
#define i_std_DOL DOL[0],DOLB[0],DOLD[0]
#define i_std_DPM DPM[0],DPMB[0],DPMD[0]
#define i_std_EER EER[0],EERB[0],EERD[0]
#define i_std_DCLR DCLR[0],DCLRB[0],DCLRD[0]
#define i_std_DOLR DOLR[0],DOLRB[0],DOLRD[0]
#define i_std_RPM RPM[0],RPMB[0],RPMD[0]

#define i_movfr_EE EE[1],EEB[1],EED[1]
#define i_movfr_DCL DCL[1],DCLB[1],DCLD[1]
#define i_movfr_DOL DOL[1],DOLB[1],DOLD[1]
#define i_movfr_DPM DPM[1],DPMB[1],DPMD[1]
#define i_movfr_EER EER[2],EERB[2],EERD[2]
#define i_movfr_DCLR DCLR[2],DCLRB[2],DCLRD[2]
#define i_movfr_DOLR DOLR[2],DOLRB[2],DOLRD[2]
#define i_movfr_RPM RPM[2],RPMB[2],RPMD[2]

#define o_FL FL[io_sel],FLB[io_sel],FLD[io_sel]
#define o_FLH FLH[flh_io_sel],FLHB[flh_io_sel],FLHD[flh_io_sel]
#define o_F1ML F1ML[ct_io_sel],F1MLB[ct_io_sel],F1MLD[ct_io_sel]
#define o_F1AL F1AL[ct_io_sel],F1ALB[ct_io_sel],F1ALD[ct_io_sel]
#define o_DZDE DZDE[io_sel],DZDEB[io_sel],DZDED[io_sel]

#define o_EML EML[ct_io_sel],EMLB[ct_io_sel],EMLD[ct_io_sel]
#define o_EMLH EMLH[emlh_io_sel],EMLHB[emlh_io_sel],EMLHD[emlh_io_sel]
#define o_EPLH EPLH[eplh_io_sel],EPLHB[eplh_io_sel],EPLHD[eplh_io_sel]
#define o_EPRL EPRL[eplh_io_sel],EPRLB[eplh_io_sel],EPRLD[eplh_io_sel]
#define o_EPSL EPSL[eplh_io_sel],EPSLB[eplh_io_sel],EPSLD[eplh_io_sel]
#define o_DO DO[fdoor_io_sel],DOB[fdoor_io_sel],DOD[fdoor_io_sel]
#define o_DC DC[fdoor_io_sel],DCB[fdoor_io_sel],DCD[fdoor_io_sel]
#define o_NUD NUD[fdoor_io_sel],NUDB[fdoor_io_sel],NUDD[fdoor_io_sel]
#define o_DOR DOR[rdoor_io_sel],DORB[rdoor_io_sel],DORD[rdoor_io_sel]
#define o_DCR DCR[rdoor_io_sel],DCRB[rdoor_io_sel],DCRD[rdoor_io_sel]
#define o_NUDR NUDR[rdoor_io_sel],NUDRB[rdoor_io_sel],NUDRD[rdoor_io_sel]
#define o_FBZ FBZ[fbz_io_sel],FBZB[fbz_io_sel],FBZD[fbz_io_sel]
#define o_HUGO HUGO[io_sel],HUGOB[io_sel],HUGOD[io_sel]
#define o_HUGA HUGA[io_sel],HUGAB[io_sel],HUGAD[io_sel]
#define o_BRK BRK,BRKB,BRKD
#define o_BRKC BRKC,BRKCB,BRKCD
#define o_EBK1 EBK1,EBK1B,EBK1D
#define o_EBK2 EBK2,EBK2B,EBK2D
#define o_EBK EBK,EBKB,EBKD
#define o_EBKC EBKC,EBKCB,EBKCD
#define o_EBA1 EBA1,EBA1B,EBA1D
#define o_EBA2 EBA2,EBA2B,EBA2D
#define o_EBB1 EBB1,EBB1B,EBB1D
#define o_EBB2 EBB2,EBB2B,EBB2D
#define o_EBC EBC,EBCB,EBCD
#define o_UPML UPML,UPMLB,UPMLD
#define o_DNML DNML,DNMLB,DNMLD
#define o_DTR DTR[io_sel],DTRB[io_sel],DTRD[io_sel]
#define o_SFST DTR2[io_sel],DTR2B[io_sel],DTR2D[io_sel]
#define o_ELOO ELOO,ELOOB,ELOOD
#define o_NPD NPD,NPDB,NPDD
#define o_EPD EPD,EPDB,EPDD
#define o_EPO EPO,EPOB,EPOD
#define o_CUL CUL[clant_io_sel],CULB[clant_io_sel],CULD[clant_io_sel]
#define o_CDL CDL[clant_io_sel],CDLB[clant_io_sel],CDLD[clant_io_sel]
#define o_CULR CULR[rclant_io_sel],CULRB[rclant_io_sel],CULRD[rclant_io_sel]
#define o_CDLR CDLR[rclant_io_sel],CDLRB[rclant_io_sel],CDLRD[rclant_io_sel]
#define o_LU LU,LUB,LUD
#define o_LD LD,LDB,LDD
#define o_DBC DBC,DBCB,DBCD
#define o_GEN GEN,GENB,GEND
#define o_DNO GEN,GENB,GEND
#define o_MTB MTB,MTBB,MTBD
#define o_GBL GBL,GBLB,GBLD
#define o_LBL LBL,LBLB,LBLD
#define o_GOVR GOVR,GOVRB,GOVRD
#define o_GOVT GOVT,GOVTB,GOVTD

#define o_std_DO DO[0],DOB[0],DOD[0]
#define o_std_DC DC[0],DCB[0],DCD[0]
#define o_std_NUD NUD[0],NUDB[0],NUDD[0]
#define o_std_DOR DOR[0],DORB[0],DORD[0]
#define o_std_DCR DCR[0],DCRB[0],DCRD[0]
#define o_std_NUDR NUDR[0],NUDRB[0],NUDRD[0]

#define o_movfr_DO DO[1],DOB[1],DOD[1]
#define o_movfr_DC DC[1],DCB[1],DCD[1]
#define o_movfr_NUD NUD[1],NUDB[1],NUDD[1]
#define o_movfr_DOR DOR[2],DORB[2],DORD[2]
#define o_movfr_DCR DCR[2],DCRB[2],DCRD[2]
#define o_movfr_NUDR NUDR[2],NUDRB[2],NUDRD[2]

#define o_MST MST,MSTB,MSTD
#define o_RUN RUN,RUNB,RUND
#define o_RUNA RUNA,RUNAB,RUNAD
#define o_MCC MCC,MCCB,MCCD
#define o_MCA MCA,MCAB,MCAD
#define o_UP UP,UPB,UPD
#define o_DNR DNR,DNRB,DNRD
#define o_UPF UPF,UPFB,UPFD
#define o_DF DF,DFB,DFD
#define o_FF FF,FFB,FFD
#define o_GR1R GR1R,GR1RB,GR1RD
#define o_GR2R GR2R,GR2RB,GR2RD
#define o_LE LE,LEB,LED
#define o_LE1 LE1,LE1B,LE1D
#define o_UTM UTM,UTMB,UTMD
#define o_FST FST,FSTB,FSTD
#define o_FSTP FSTP,FSTPB,FSTPD
#define o_PFC PFC,PFCB,PFCD
#define o_RCM RCM[rcm_io_sel],RCMB[rcm_io_sel],RCMD[rcm_io_sel]
#define o_RCF RCF,RCFB,RCFD
#define o_DDAC DDAC[io_sel],DDACB[io_sel],DDACD[io_sel]
#define o_UDAC UDAC[io_sel],UDACB[io_sel],UDACD[io_sel]
#define o_ATDL ATDL,ATDLB,ATDLD
#define o_ATUL ATUL,ATULB,ATULD
#define o_TDOL TDOL,TDOLB,TDOLD
#define o_TDCL TDCL,TDCLB,TDCLD
#define o_TDOLR TDOLR,TDOLRB,TDOLRD
#define o_TDCLR TDCLR,TDCLRB,TDCLRD
#define o_TAVL TAVL,TAVLB,TAVLD
#define o_TAVL2 TAVL2,TAVL2B,TAVL2D
#define o_TIND TIND,TINDB,TINDD
#define o_TMED TMED,TMEDB,TMEDD
#define o_TFSO TFSO,TFSOB,TFSOD
#define o_TP1 TP1,TP1B,TP1D
#define o_TP2 TP2,TP2B,TP2D
#define o_TP3 TP3,TP3B,TP3D
#define o_TP4 TP4,TP4B,TP4D
#define o_TP5 TP5,TP5B,TP5D
#define o_TP6 TP6,TP6B,TP6D
#define o_EQL EQL,EQLB,EQLD
#define o_EQLC EQLC[ct_io_sel],EQLCB[ct_io_sel],EQLCD[ct_io_sel]
#define o_MBC MBC[mbc_io_sel],MBCB[mbc_io_sel],MBCD[mbc_io_sel]
#define o_MBP MBP,MBPB,MBPD
#define o_MB1 MB1,MB1B,MB1D
#define o_MB2 MB2,MB2B,MB2D
#define o_RST RST,RSTB,RSTD
#define o_EDL EDL[io_sel],EDLB[io_sel],EDLD[io_sel]
#define o_EDHL EDHL,EDHLB,EDHLD
#define o_FSDO FSDO,FSDOB,FSDOD
#define o_RSDO RSDO,RSDOB,RSDOD
#define o_RVDR RVDR[rdoor_io_sel],RVDRB[rdoor_io_sel],RVDRD[rdoor_io_sel]
#define o_REVR REVR[rdoor_io_sel],REVRB[rdoor_io_sel],REVRD[rdoor_io_sel]
#define o_RVD RVD[ct_io_sel],RVDB[ct_io_sel],RVDD[ct_io_sel]
#define o_REV REV[io_sel],REVB[io_sel],REVD[io_sel]
#define o_HVD HVD[hvd_io_sel],HVDB[hvd_io_sel],HVDD[hvd_io_sel]
#define o_HVDR HVDR[rdoor_io_sel],HVDRB[rdoor_io_sel],HVDRD[rdoor_io_sel]
#define o_CLF CLF[io_sel],CLFB[io_sel],CLFD[io_sel]
#define o_FSO FSO,FSOB,FSOD
#define o_FiFL FiFL,FiFLB,FiFLD
#define o_FLCT FLCT,FLCTB,FLCTD
#define o_FBNBCT FBNBCT,FBNBCTB,FBNBCTD
#define o_FBNB FBNB[io_sel],FBNBB[io_sel],FBNBD[io_sel]
#define o_DCA DCA[ct_io_sel],DCAB[ct_io_sel],DCAD[ct_io_sel]
#define o_HB HB[io_sel],HBB[io_sel],HBD[io_sel]
#define o_SABUZ SABUZ[io_sel],SABUZB[io_sel],SABUZD[io_sel]
#define o_SABEE SABEE[io_sel],SABEEB[io_sel],SABEED[io_sel]
#define o_SABO SABO,SABOB,SABOD
#define o_ISER ISER[iserv_io_sel],ISERB[iserv_io_sel],ISERD[iserv_io_sel]
#define o_OSERB OSERB[0],OSERBB[0],OSERBD[0]
#define o_OSERL OSERL[oserl_io_sel],OSERLB[oserl_io_sel],OSERLD[oserl_io_sel]
#define o_OPENL OPENL,OPENLB,OPENLD
#define o_INSEC INSEC,INSECB,INSECD
#define o_FSOCS FSOCS,FSOCSB,FSOCSD
#define o_FSORS FSORS,FSORSB,FSORSD
#define o_FW FW,FWB,FWD
#define o_DPI1 DPI1[io_sel],DPI1B[io_sel],DPI1D[io_sel]
#define o_DPI2 DPI2[io_sel],DPI2B[io_sel],DPI2D[io_sel]
#define o_DPI3 DPI3[io_sel],DPI3B[io_sel],DPI3D[io_sel]
#define o_DPI4 DPI4[io_sel],DPI4B[io_sel],DPI4D[io_sel]
#define o_DPI5 DPI5[io_sel],DPI5B[io_sel],DPI5D[io_sel]
#define o_DPI6 DPI6[io_sel],DPI6B[io_sel],DPI6D[io_sel]
#define o_OLL OLL[ct_io_sel],OLLB[ct_io_sel],OLLD[ct_io_sel]
#define o_SERV SERVO[servo_io_sel],SERVOB[servo_io_sel],SERVOD[servo_io_sel]

#define o_HDB HDB,HDBB,HDBD
#define o_DESL DESL,DESLB,DESLD
#define o_INSP INSP,INSPB,INSPD
#define o_ADO ADO,ADOB,ADOD
#define o_RADO RADO,RADOB,RADOD
#define o_PH1R PH1R,PH1RB,PH1RD
#define o_PH2HD PH2HD,PH2HDB,PH2HDD
#define o_PH2ON PH2ON,PH2ONB,PH2OND
#define o_PH2OF PH2OF,PH2OFB,PH2OFD
#define o_DAO DAO,DAOB,DAOD
#define o_RC RC,RCB,RCD
#define o_RDT RDT,RDTB,RDTD
#define o_MPTR MPTR,MPTRB,MPTRD
#define o_CODG CODG,CODGB,CODGD
#define o_COHS COHS,COHSB,COHSD
#define o_ATO ATO,ATOB,ATOD
#define o_IAO IAO,IAOB,IAOD
#define o_FPH1 FPH1,FPH1B,FPH1D
#define o_RAHS RAHS,RAHSB,RAHSD
#define o_FPH2 FPH2,FPH2B,FPH2D
#define o_DSC DSC,DSCB,DSCD
#define o_FDOB FDOB,FDOBB,FDOBD
#define o_RDOB RDOBR,RDOBB,RDOBD
#define o_TC TC_,TC_B,TC_D
#define o_FRCL FRCL,FRCLB,FRCLD
#define o_HCO HCO,HCOB,HCOD
#define o_FH FH,FHB,FHD
#define o_FPS2 FPS2,FPS2B,FPS2D
#define o_EFST EFST,EFSTB,EFSTD
#define o_EMS1Z EMS1Z,EMS1ZB,EMS1ZD
#define o_EMS2Z EMS2Z,EMS2ZB,EMS2ZD
#define o_EMS3Z EMS3Z,EMS3ZB,EMS3ZD
#define o_EMS4Z EMS4Z,EMS4ZB,EMS4ZD
#define o_EMS5Z EMS5Z,EMS5ZB,EMS5ZD
#define o_EMS6Z EMS6Z,EMS6ZB,EMS6ZD
#define o_EMS7Z EMS7Z,EMS7ZB,EMS7ZD
#define o_EMS8Z EMS8Z,EMS8ZB,EMS8ZD
#define o_EMS9Z EMS9Z,EMS9ZB,EMS9ZD
#define o_EMS10Z EMS10Z,EMS10ZB,EMS10ZD
#define o_EMS11Z EMS11Z,EMS11ZB,EMS11ZD
#define o_FRT1Z FRT1Z,FRT1ZB,FRT1ZD
#define o_FRT2Z FRT2Z,FRT2ZB,FRT2ZD
#define o_FRT3Z FRT3Z,FRT3ZB,FRT3ZD
#define o_PDO PDO,PDOB,PDOD
#define o_PDOR PDOR,PDORB,PDORD
#define o_G_INSP G_INSP,G_INSPB,G_INSPD
#define o_G_ADO G_ADO,G_ADOB,G_ADOD
#define o_G_FDCBO G_FDCBO,G_FDCBOB,G_FDCBOD
#define o_G_RDCBO G_RDCBO,G_RDCBOB,G_RDCBOD
#define o_G_ADOH G_ADOH,G_ADOHB,G_ADOHD
#define o_G_RDO G_RDO,G_RDOB,G_RDOD
#define o_G_RDOH G_RDOH,G_RDOHB,G_RDOHD
#define o_G_RC G_RC,G_RCB,G_RCD
#define o_G_SOPEN G_SOPEN,G_SOPENB,G_SOPEND
#define o_G_ININD G_ININD,G_ININDB,G_ININDD
#define o_G_FH1R G_FH1R,G_FH1RB,G_FH1RD
#define o_G_PH2HD G_PH2HD,G_PH2HDB,G_PH2HDD
#define o_G_PH2ON G_PH2ON,G_PH2ONB,G_PH2OND
#define o_G_PH2OF G_PH2OF,G_PH2OFB,G_PH2OFD
#define o_RUDA RUDA,RUDAB,RUDAD
#define o_RDDA RDDA,RDDAB,RDDAD
#define o_RFSO RFSO,RFSOB,RFSOD
#define o_RPI1 RPI1,RPI1B,RPI1D
#define o_RPI2 RPI2,RPI2B,RPI2D
#define o_RPI3 RPI3,RPI3B,RPI3D
#define o_RPI4 RPI4,RPI4B,RPI4D
#define o_RPI5 RPI5,RPI5B,RPI5D
#define o_RPI6 RPI6,RPI6B,RPI6D
#define o_RPI7 RPI7,RPI7B,RPI7D
#define o_RPI8 RPI8,RPI8B,RPI8D
#define o_RPI9 RPI8,RPI9B,RPI9D
#define o_RPI10 RPI10,RPI10B,RPI10D
#define o_RPI11 RPI11,RPI11B,RPI11D
#define o_RPI12 RPI12,RPI12B,RPI12D
#define o_RPI13 RPI13,RPI13B,RPI13D
#define o_RPI14 RPI14,RPI14B,RPI14D
#define o_RPI15 RPI15,RPI15B,RPI15D
#define o_RPI16 RPI16,RPI16B,RPI16D
#define o_RPI17 RPI17,RPI17B,RPI17D
#define o_RPI18 RPI18,RPI18B,RPI18D
#define o_RPI19 RPI19,RPI19B,RPI19D
#define o_RPI20 RPI20,RPI20B,RPI20D
#define o_RPI21 RPI21,RPI21B,RPI21D


