//*********************************************************************************************
// MAIN I/O BOARD
//*********************************************************************************************

#define i_AD &si_AD 
#define i_IND &si_IND 
#define i_LBP &si_LBP 
#define i_GBP &si_GBP 
#define i_L120 &si_L120		//"formerly LPH" 
#define i_S10 &si_S10 		// Controller main 120 VAC power
#define i_FEP &si_FEP
#define i_EBK &si_EBK 	    // Emergency brake coil input
#define i_GOV &si_GOV 

#define i_TF &si_TF 
#define i_BF &si_BF 
#define i_PS &si_PS 
#define i_HSS &si_HSS 
#define i_ACC &si_ACC 
#define i_BAD &si_BAD 
#define i_BAU &si_BAU 
#define i_TAD &si_TAD 

#define i_TAU &si_TAU 
#define i_DLB &si_DLB 
#define i_DLB_1 &si_DLB_1 
#define i_DLM &si_DLM 
#define i_DLM_1 &si_DLM_1 
#define i_DLT &si_DLT 
#define i_DLT_1 &si_DLT_1 
#define i_RLM &si_RLM 

#define i_RLM_1 &si_RLM_1 
#define i_TDC &si_TDC 
#define i_MDC &si_MDC 
#define i_BDC &si_BDC 
#define i_MDCR &si_MDCR 
#define i_BKS &si_BKS 
#define i_EBKS &si_EBKS 	// Emergency Brake Switch input
#define i_SPI &si_SPI 

#define i_MRIU &si_MRIU 
#define i_MRIN &si_MRIN 
#define i_AUTO &si_AUTO 
#define i_MRSW &si_MRSW 
#define i_ICA &si_ICA 
#define i_MRID &si_MRID 
#define i_MRIE &si_MRIE 	// Machine room inspection input

#define i_CEN &si_CEN
#define i_SPD &si_SPD
#define i_CFLT &si_CFLT
#define i_L120B &si_L120B
#define i_NTSF &si_NTSF
#define i_PALF &si_PALF
#define i_TSTM &si_TSTM
#define i_RDOOR &si_RDOOR
#define i_TSD &si_TSD


#define i_READY &si_READY 	// Drive Ready input
#define i_GTS &si_GTS 	// Gripper Trip Switch input
#define i_GRT1 &si_GRT1 
#define i_GRT2 &si_GRT2 
#define i_RGS_1 &si_RGS_1 
#define i_GS &si_GS 
#define i_GS_1 &si_GS_1 
#define i_FST &si_FST 

#define i_DN &si_DN 		// Down Directional Stop
#define i_DZ &si_DZ 		// Door Zone input (DZU AND DZD)
#define i_DZ_1 &si_DZ_1 	// Door Zone input (DZU AND DZD)
#define i_RGS &si_RGS 
#define i_CTA &si_CTA 
#define i_DTS &si_DTS 	// Down Emergeny Terminal Slowdown
#define i_UN &si_UN 		// Up Directional Stop
#define i_UTS &si_UTS 	// Up Emergency Terminal Slowdown 

#define i_ID &si_ID 
#define i_INE &si_INE 
#define i_IU &si_IU 
#define i_INS &si_INS 
#define i_CTIN &si_INS 
#define i_P &si_P 
#define i_ICI &si_ICI 
#define i_FFS &si_FFS 

#define i_MCC &si_MCC 
#define i_MCA &si_MCA 
#define i_DON &si_DON 
#define i_BRK &si_BRK 
#define i_RUN &si_RUN 
#define i_MCX &si_MCX 
#define i_RUNA &si_RUNA 
#define i_RUNX &si_RUNX 

#define i_SD &si_SD 
#define i_SDF &si_SDF 
#define i_SU &si_SU 
#define i_SUF &si_SUF 
#define i_NTS &si_NTS
#define i_ETS &si_ETS

#define o_CEN &so_CEN
#define o_MTO &so_MTO
#define o_STE &so_STE
#define o_UL &so_UL
#define o_DL &so_DL
#define o_DZ &so_DZ
#define o_SELOK &so_SELOK
#define o_SP75 &so_SP75
#define o_SP150 &so_SP150
#define o_HWLRN &so_HWLRN

#define o_FLH &so_FLH 
#define o_GR2R &so_GR2R 
#define o_LE &so_LE 
#define o_LE1 &so_LE1 
#define o_FST &so_FST 
#define o_FSTP &so_FSTP 

#define o_RST &so_RST 
#define o_MCC &so_MCC 
#define o_MCA &so_MCA 
#define o_PFC &so_PFC 
#define o_DBG &so_DBG 
#define o_FF &so_FF 
#define o_BUZ &so_BUZ 
#define o_EBK1 &so_EBK1 

#define o_RUNA &so_RUNA 
#define o_RUN &so_RUN 
#define o_SD &so_SD 
#define o_SDF &so_SDF 
#define o_SU &so_SU 
#define o_SUF &so_SUF 

#define o_BRK &so_BRK 
#define o_EBK2 &so_EBK2 
#define o_DBC &so_DBC 
	


//*********************************************************************************************
// Machine Room Can I/O
//*********************************************************************************************

#define i_EBK1 &si_EBK1 	// Emergency brake 1 coil input
#define i_EBK2 &si_EBK2 	// Emergency brake 2 coil input
#define i_EBA &si_EBA 	    // Emergency brake A coil input
#define i_EBB &si_EBB 	    // Emergency brake B coil input

#define i_TPL &si_TPL 
#define i_TPH &si_TPH 

#define i_SEE &si_SEE 
#define i_TEE &si_TEE 
#define i_CMP &si_CMP 
#define i_BFS &si_BFS 
#define i_PDS &si_PDS 
#define i_HMO &si_HMO 

#define i_TGDO &si_TGDO 
#define i_TGDOR &si_TGDOR 
#define i_TGDC &si_TGDC 
#define i_TGDCR &si_TGDCR 
#define i_CCPBS &si_CCPBS 

#define i_XBKS1 &si_XBKS1 
#define i_BKS1 &si_BKS1  
#define i_BKS2 &si_BKS2 
#define i_BDBS &si_BDBS 


#define i_RM_ED &si_RM_ED 
#define i_RM_EDR &si_RM_EDR 
#define i_RM_INDC &si_RM_INDC  
#define i_OSERA &si_OSERA 
#define i_SAB &si_SAB 
#define i_SECFM &si_SECFM 

#define i_SCSMR &si_SCSMR 
#define i_SCSRMR &si_SCSRMR 

#define i_BP1 &si_BP1 
#define i_BP2 &si_BP2 
#define i_BP4 &si_BP4 
#define i_BP8 &si_BP8 
#define i_BP16 &si_BP16 
#define i_BP32 &si_BP32 


#define i_LPS &si_LPS 
#define i_LOS &si_LOS 
#define i_RM_DOB &si_RM_DOB 
#define i_RM_DCB &si_RM_DCB 
#define i_RM_DCBR &si_RM_DCBR 
#define i_RM_DOBR &si_RM_DOBR 

#define i_EMP &si_EMP 
#define i_EPS &si_EPS 
#define i_EPT &si_EPT 
#define i_HEOF &si_HEOF 
#define i_HEOFR &si_HEOFR 
#define i_JAILB &si_JAILB 
#define i_HUGI &si_HUGI 
#define i_HUGB &si_HUGB 
#define i_RTL &si_RTL 
#define i_RTLH &si_RTLH  
#define i_RTLDC &si_RTLDC 
#define i_EMSH &si_EMSH 

#define i_THDS &si_THDS 

#define i_FS &si_FS 
#define i_BP &si_BP 
#define i_FSX &si_FSX 
#define i_ALT &si_ALT  
#define i_MES &si_MES 
#define i_MRS &si_MRS 
#define i_HWS &si_HWS 
#define i_HWS2 &si_HWS2 


#define i_EQR &si_EQR 
#define i_ETH &si_ETH 
#define i_COL &si_COL 


#define i_CWS &si_CWS 
#define i_ELOF &si_ELOF 
#define i_HC &si_HC 
#define i_CCP &si_CCP 
#define i_CCLP &si_CCLP 
#define i_HCLP &si_HCLP 
#define i_LHCP &si_LHCP 
#define i_DMO &si_DMO 
#define o_ISPD &so_ISPD 


#define o_BRKC &so_BRKC 
#define o_EBK &so_EBK 
#define o_EBKC &so_EBKC 
#define o_EBA1 &so_EBA1 
#define o_EBA2 &so_EBA2 
#define o_EBB1 &so_EBB1 
#define o_EBB2 &so_EBB2 
#define o_EBC &so_EBC 
#define o_UPML &so_UPML 
#define o_DNML &so_DNML 
#define o_NPL &so_NPL 
#define o_EMLH &so_EMLH 
#define o_EPLH &so_EPLH 
#define o_EPRL &so_EPRL 
#define o_EPSL &so_EPSL 
#define o_HUGO &so_HUGO 
#define o_HUGA &so_HUGA 
#define o_NPD &so_NPD 
#define o_EPD &so_EPD 
#define o_EPO &so_EPO 
#define o_TCU &so_TCU 
#define o_LU &so_LU 
#define o_LD &so_LD 
#define o_LUR &so_LUR 
#define o_LDR &so_LDR 
#define o_GEN &so_GEN 
#define o_DNO &so_DNO 
#define o_MTB &so_MTB 
#define o_GBL &so_GBL 
#define o_LBL &so_LBL 
#define o_GOVR &so_GOVR 
#define o_GOVT &so_GOVT 


#define o_RCF &so_RCF 
#define o_MST &so_MST 
#define o_GR1R &so_GR1R 
#define o_UTM &so_UTM 
#define o_TDOL &so_TDOL 
#define o_TDCL &so_TDCL 
#define o_TDOLR &so_TDOLR 
#define o_TDCLR &so_TDCLR 
#define o_TAVL &so_TAVL 
#define o_TAVL2 &so_TAVL2 
#define o_TIND &so_TIND 
#define o_TMED &so_TMED 
#define o_TFSO &so_TFSO 
#define o_TP1 &so_TP1 
#define o_TP2 &so_TP2 
#define o_TP3 &so_TP3 
#define o_TP4 &so_TP4 
#define o_TP5 &so_TP5 
#define o_TP6 &so_TP6 
#define o_EQL &so_EQL 
#define o_MBC &so_MBC 
#define o_MBP &so_MBP 
#define o_MB1 &so_MB1 
#define o_MB2 &so_MB2 
#define o_FSDO &so_FSDO 
#define o_RSDO &so_RSDO 
#define o_RVDR &so_RVDR 
#define o_REVR &so_REVR 
#define o_RVD &so_RVD 
#define o_REV &so_REV 
#define o_HVD &so_HVD 
#define o_HVDR &so_HVDR 


#define o_FSO &so_FSO 
#define o_FiFL &so_FiFL 
#define o_FLCT &so_FLCT 
#define o_FBNBCT &so_FBNBCT 
#define o_FBNB &so_FBNB 
#define o_DCA &so_DCA 
#define o_HB &so_HB 
#define o_SABUZ &so_SABUZ 
#define o_SABEE &so_SABEE 
#define o_SABO &so_SABO 
#define o_ISER &so_ISER 
#define o_INSO &so_INSO 
#define o_OSERB &so_OSERB 
#define o_OSERL &so_OSERL 
#define o_OPENL &so_OPENL 
#define o_INSEC &so_INSEC 
#define o_FSOCS &so_FSOCS 
#define o_FSORS &so_FSORS 
#define o_FW &so_FW 
#define o_DDA &so_DDA 
#define o_UDA &so_UDA 
#define o_DPI1 &so_DPI1 
#define o_DPI2 &so_DPI2 
#define o_DPI3 &so_DPI3 
#define o_DPI4 &so_DPI4 
#define o_DPI5 &so_DPI5 
#define o_DPI6 &so_DPI6 
#define o_OLL &so_OLL 
#define o_HDB &so_HDB 
#define o_DESL &so_DESL 
#define o_INSP &so_INSP 
#define o_ADO &so_ADO 
#define o_RADO &so_RADO 
#define o_PH1R &so_PH1R 
#define o_PH2HD &so_PH2HD 
#define o_PH2ON &so_PH2ON 
#define o_PH2OF &so_PH2OF 
#define o_DAO &so_DAO 
#define o_RC &so_RC 
#define o_SQUEZ &so_SQUEZ 
#define o_RDT &so_RDT 
#define o_MPTR &so_MPTR 
#define o_CODG &so_CODG  			// Door/Gate Cut-out output for courion door interface
#define o_COHS &so_COHS 
#define o_ATO &so_ATO 
#define o_IAO &so_IAO 
#define o_FPH1 &so_FPH1 
#define o_RAHS &so_RAHS 
#define o_FPH2 &so_FPH2 
#define o_DSC &so_DSC 
#define o_FDOB &so_FDOB 
#define o_RDOB &so_RDOB 
#define o_TC &so_TC 
#define o_FRCL &so_FRCL 
#define o_HCO &so_HCO 
#define o_FH &so_FH 
#define o_FPS2 &so_FPS2 

#define o_EFST &so_EFST 
#define o_EMS1Z &so_EMS1Z 
#define o_EMS2Z &so_EMS2Z 
#define o_EMS3Z &so_EMS3Z 
#define o_EMS4Z &so_EMS4Z 
#define o_EMS5Z &so_EMS5Z 
#define o_EMS6Z &so_EMS6Z 
#define o_EMS7Z &so_EMS7Z 
#define o_EMS8Z &so_EMS8Z 
#define o_EMS9Z &so_EMS9Z 
#define o_EMS10Z &so_EMS10Z 
#define o_EMS11Z &so_EMS11Z 
#define o_FRT1Z &so_FRT1Z 
#define o_FRT2Z &so_FRT2Z 
#define o_FRT3Z &so_FRT3Z 
#define o_PDO &so_PDO 
#define o_PDOR &so_PDOR 
#define o_G_INSP &so_G_INSP 
#define o_G_ADO &so_G_ADO 
#define o_G_FDCBO &so_G_FDCBO 
#define o_G_RDCBO &so_G_RDCBO 
#define o_G_ADOH &so_G_ADOH 
#define o_G_RDO &so_G_RDO 
#define o_G_RDOH &so_G_RDOH 
#define o_G_RC &so_G_RC 
#define o_G_SOPEN &so_G_SOPEN 
#define o_G_ININD &so_G_ININD 
#define o_G_FH1R &so_G_FH1R 
#define o_G_PH2HD &so_G_PH2HD 
#define o_G_PH2ON &so_G_PH2ON 
#define o_G_PH2OF &so_G_PH2OF 

#define o_RUDA &so_RUDA 
#define o_RDDA &so_RDDA 
#define o_RFSO &so_RFSO 
#define o_RPI1 &so_RPI1
#define o_RPI2 &so_RPI2
#define o_RPI3 &so_RPI3
#define o_RPI4 &so_RPI4
#define o_RPI5 &so_RPI5
#define o_RPI6 &so_RPI6
#define o_RPI7 &so_RPI7
#define o_RPI8 &so_RPI8
#define o_RPI9 &so_RPI9
#define o_RPI10 &so_RPI10
#define o_RPI11 &so_RPI11
#define o_RPI12 &so_RPI12
#define o_RPI13 &so_RPI13
#define o_RPI14 &so_RPI14
#define o_RPI15 &so_RPI15
#define o_RPI16 &so_RPI16
#define o_RPI17 &so_RPI17
#define o_RPI18 &so_RPI18
#define o_RPI19 &so_RPI19
#define o_RPI20 &so_RPI20
#define o_RPI21 &so_RPI21

#define o_SERV &so_SERV
#define o_SERVC &so_SERVC

#define i_SECUR &si_SECUR
#define i_RSECU &si_RSECU
#define i_OSEC &si_OSEC
#define i_ORSEC &si_ORSEC
#define i_ALTPI &si_ALTPI

#define o_MESO &so_MESO
#define o_ALTO &so_ALTO
#define i_SMK(fl) &si_SMK[fl]

#define i_RDZE &si_RDZE 
#define o_PDCA &so_PDCA
#define o_PFDCA &so_PFDCA

#define i_FETST &si_FETST
#define i_RETST &si_RETST
#define o_FETST &so_FETST
#define o_RETST &so_RETST
#define i_EE2 &si_EE2
#define i_EER2 &si_EER2
#define i_HDOB &si_HDOB
#define i_HDCB &si_HDCB
#define i_HDOBR &si_HDOBR
#define i_HDCBR &si_HDCBR
#define o_OSERD &so_OSERD

#define i_SCD &si_SCD
#define i_RSPD &si_RSPD

#define o_ADOF(fl) &so_ADOF[fl]
#define o_ADOR(fl) &so_ADOR[fl]

#define i_MSP1 &si_MSPI1 
#define i_MSP2 &si_MSPI2 
#define i_MSP3 &si_MSPI3 
#define o_MSP1 &so_MSPO1
#define o_MSP2 &so_MSPO2
#define o_MSP3 &so_MSPO3


//ZYM 10/18/2017
//input
#define i_MLi0 &si_MLi0 
#define i_MLi1 &si_MLi1 
#define i_MLi2 &si_MLi2 
#define i_MLi3 &si_MLi3 
#define i_MLi4 &si_MLi4 
#define i_MLi5 &si_MLi5 
#define i_MLi6 &si_MLi6 
#define i_MLi7 &si_MLi7 
#define i_MLi8 &si_MLi8 
#define i_MLi9 &si_MLi9 
#define i_MLi10 &si_MLi10 
#define i_MLi11 &si_MLi11 

//output
#define o_MLo0 &so_MLo0 
#define o_MLo1 &so_MLo1 
#define o_MLo2 &so_MLo2 
#define o_MLo3 &so_MLo3 
#define o_MLo4 &so_MLo4 
#define o_MLo5 &so_MLo5 
#define o_MLo6 &so_MLo6 
#define o_MLo7 &so_MLo7 
#define o_MLo8 &so_MLo8 
#define o_MLo9 &so_MLo9 
#define o_MLo10 &so_MLo10 
#define o_MLo11 &so_MLo11 

//input
#define i_MRi0 &si_MRi0 
#define i_MRi1 &si_MRi1 
#define i_MRi2 &si_MRi2 
#define i_MRi3 &si_MRi3 
#define i_MRi4 &si_MRi4 
#define i_MRi5 &si_MRi5 
#define i_MRi6 &si_MRi6 
#define i_MRi7 &si_MRi7 
#define i_MRi8 &si_MRi8 
#define i_MRi9 &si_MRi9 
#define i_MRi10 &si_MRi10 
#define i_MRi11 &si_MRi11 

//output
#define o_MRo0 &so_MRo0 
#define o_MRo1 &so_MRo1 
#define o_MRo2 &so_MRo2 
#define o_MRo3 &so_MRo3 
#define o_MRo4 &so_MRo4 
#define o_MRo5 &so_MRo5 
#define o_MRo6 &so_MRo6 
#define o_MRo7 &so_MRo7 
#define o_MRo8 &so_MRo8 
#define o_MRo9 &so_MRo9 
#define o_MRo10 &so_MRo10 
#define o_MRo11 &so_MRo11 



//ZYM 11/20/2017
//input
#define i_suf &si_suf 
#define i_sufc &si_sufc 
#define i_su &si_su
#define i_suc &si_suc 
#define i_sdf &si_sdf 
#define i_sdfc &si_sdfc 
#define i_sd &si_sd 
#define i_sdc &si_sdc 
#define i_s6s10 &si_s6s10 

//output
#define o_S5LPS &so_S5LPS
#define o_S4LOS &so_S4LOS 
#define o_S3TPL &so_S3TPL 



//input
#define i_s2mrw &si_s2mrw
#define i_24s10 &si_24s10 
#define i_22gov &si_22gov
#define i_20tlh &si_20tlh 
#define i_h18ps &si_h18ps 
#define i_16mrw &si_16mrw 

//output
#define o_S1TPH &so_S1TPH 
#define o_23GOV &so_23GOV 
#define o_21TLH &so_21TLH
#define o_H19PS &so_H19PS 
#define o_17HSS &so_17HSS



//input
#define i_14mrw &si_14mrw
#define i_11mrw &si_11mrw 
#define i_h8s10 &si_h8s10
#define i_h6s10 &si_h6s10 
#define i_h4s10 &si_h4s10 

//output
#define o_15TSD &so_15TSD 
#define o_13BAU &so_13BAU 
#define o_12BAD &so_12BAD
#define o_10TAU &so_10TAU 
#define o_H9TAD &so_H9TAD
#define o_H7DLB &so_H7DLB 
#define o_H5DLM &so_H5DLM



//input
#define i_h2s10 &si_h2s10
#define i_mrsw &si_mrsw 
#define i_mc &si_mc
#define i_mc1 &si_mc1
#define i_ste &si_ste 
#define i_s10 &si_s10 

//output
#define o_H3DLT &so_H3DLT 
#define o_RLM &so_RLM 
//-#define o_MC1 &so_MC1
#define o_0MTO &so_0MTO 
#define o_0SPD &so_0SPD
#define o_0CFLT &so_0CFLT 
#define o_TMRLY &so_TMRLY 



//input
#define i_18hss &si_18hss

//output
#define o_17CST &so_17CST 
#define o_16INS &so_16INS 
#define o_T15IU &so_T15IU
#define o_T14ID &so_T14ID 
#define o_13IEN &so_13IEN
#define o_12ICA &so_12ICA 
#define o_11ICI &so_11ICI 
#define o_10ACC &so_10ACC 
#define o_T9GS &so_T9GS
#define o_RGS &so_RGS
#define o_RDRLY &so_RDRLY
 

//*********************************************************************************************
// Car Top Can I/O
//*********************************************************************************************

#define i_RLS &si_RLS 
#define i_CSS &si_CSS 
#define i_CTS &si_CTS 
#define i_CST &si_CST 
#define i_DET &si_DET 
#define i_RDET &si_RDET 
#define i_HWSET &si_HWSET	// Run/Setup jumper

#define o_IFL &so_IFL 		// Inspection Fire Light
#define o_IFB &so_IFB 		// Inspection Fire Buzzer

#define o_CUL &so_CUL 		// Car Up lantern
#define o_CDL &so_CDL 		// Car Down lantern
#define o_CLF &so_CLF 		// Cab Light and Fan
#define o_FAN &so_FAN 		// cab Fan
#define o_LIGHT &so_LIGHT 	// Cab Light
#define o_SELCM &so_SELCM 	// Selector Comm LED
#define o_NTSCM &so_NTSCM 	// NTS Comm LED


#define i_DT &si_DT  		// Down Terminal Slowdosn
#define i_DT1 &si_DT1  	// 1st Down Terminal limit
#define i_DT2 &si_DT2  	// 2nd Down Terminal limit
#define i_DT3 &si_DT3  	// 3rd Down Terminal limit
#define i_DT4 &si_DT4  	// 4th Down Terminal limit
#define i_DT5 &si_DT5  	// 5th Down Terminal limit
#define i_DT6 &si_DT6  	// 6th Down Terminal limit

#define i_UT &si_UT  		// Up Terminal Slowdown
#define i_UT1 &si_UT1 		// 1st Up Terminal limit
#define i_UT2 &si_UT2 		// 2nd Up Terminal limit
#define i_UT3 &si_UT3  	// 3rd Up Terminal limit
#define i_UT4 &si_UT4  	// 4th Up Terminal Limit
#define i_UT5 &si_UT5  	// 5th Up Terminal limit
#define i_UT6 &si_UT6  	// 6th Up Terminal Limit

#define i_DL &si_DL   	// Down Level Input
#define i_DZA &si_DZA  	// Door Zone A input (DZU OR DZD)
#define i_UL &si_UL  		// Up level input
#define i_DZsel &si_DZsel 	// Door Zone Selector Count 

#define i_UL1 &si_UL1  		// Up level input
#define i_UL2 &si_UL2  		// Up level input
#define i_UL3 &si_UL3  		// Up level input
#define i_UL4 &si_UL4  		// Up level input

#define i_DL1 &si_DL1   	// Down Level Input
#define i_DL2 &si_DL2   	// Down Level Input
#define i_DL3 &si_DL3   	// Down Level Input
#define i_DL4 &si_DL4   	// Down Level Input

#define i_DZU &si_DZU 		// Door Zone Sensor input
#define i_DZD &si_DZD 		// Door Zone Sensor input
#define i_DZ1A &si_DZ1A 	// Door Zone Sensor input
#define i_DZ2A &si_DZ2A 	// Door Zone Sensor input
#define i_DZ1B &si_DZ1B 	// Door Zone Sensor input
#define i_DZ2B &si_DZ2B 	// Door Zone Sensor input

#define i_UNL &si_UNL 		// Up Normal Latch
#define i_UNsel &si_UNsel 	// Up Normal Selector Count
#define i_UTsel &si_UTsel 	// Up Terminal Slowdown Selector Count 
#define i_UTSsn &si_UTSsn 	// Up Emergency Terminal Slowdown Sensor 

#define i_DNL &si_DNL 		// Down Normal Latch
#define i_DNsel &si_DNsel 	// Down Normal Selector Count
#define i_DTsel &si_DTsel 	// Down Terminal Slowdown Selector Count
#define i_DTSsn &si_DTSsn 	// Down Emergency Terminal Slowdown Sensor

#define i_ETSsn &si_ETSsn 	// Emergency Terminal Slowdown Sensor


#define i_SE &si_SE 
#define i_SER &si_SER 
#define i_EE &si_EE 
#define i_EER &si_EER 
#define i_DCL &si_DCL 
#define i_DOL &si_DOL 
#define i_DCLR &si_DCLR 
#define i_DOLR &si_DOLR 
#define i_ALRM &si_ALRM 
#define i_ALRMR &si_ALRMR 
#define i_EMS &si_EMS 



#define i_DPM &si_DPM 
#define i_RPM &si_RPM 

#define i_DOB &si_DOB 
#define i_DCB &si_DCB 
#define i_DOBR &si_DOBR 
#define i_DCBR &si_DCBR 

#define i_FS2 &si_FS2 
#define i_FS2H &si_FS2H 
#define i_FS2C &si_FS2C 

#define i_FiDOB &si_FiDOB 
#define i_FiDOBR &si_FiDOBR 
#define i_FiFS2C &si_FiFS2C 
#define i_FiDCB &si_FiDCB 
#define i_FiDCBR &si_FiDCBR 

#define i_ICID &si_ICID 
#define i_ICIU &si_ICIU 
#define i_ED &si_ED 
#define i_EDHL &si_EDHL 
#define i_EDR &si_EDR 
#define i_OVL &si_OVL 
#define i_LWA &si_LWA 
#define i_LWB &si_LWB 
#define i_LWD &si_LWD 

#define i_ATT &si_ATT 
#define i_ATTUP &si_ATTUP 
#define i_ATTDN &si_ATTDN 
#define i_ATTBY &si_ATTBY 
#define i_ATTHC &si_ATTHC 


#define i_DZSA &si_DZSA 
#define i_UNS &si_UNS 
#define i_DNS &si_DNS 
#define i_DLW &si_DLW 
#define i_HBE &si_HBE 
#define i_HBER &si_HBER 


#define i_INDC &si_INDC 
#define i_INDS &si_INDS 
#define i_IDSO &si_IDSO 
#define i_ICVIP &si_ICVIP 
#define i_SCS &si_SCS 
#define i_SCSR &si_SCSR 
#define i_ZON &si_ZON 


#define i_DS &si_DS 
#define i_US &si_US 

									      																		
#define o_DO &so_DO 
#define o_DC &so_DC 
#define o_NUD &so_NUD 
#define o_DOR &so_DOR 
#define o_DCR &so_DCR 
#define o_NUDR &so_NUDR 
#define o_DZDE &so_DZDE 
#define o_DTR &so_DTR 
#define o_SFST &so_SFST 
#define o_HEOFL &so_HEOFL 
#define o_CULR &so_CULR 
#define o_CDLR &so_CDLR 
#define o_RCM &so_RCM 
#define o_FL &so_FL 
#define o_FS1L &so_FS1L 
#define o_FS2L &so_FS2L 
#define o_F1ML &so_F1ML 
#define o_F1AL &so_F1AL 
#define o_EML &so_EML 
#define o_ATDL &so_ATDL 
#define o_ATUL &so_ATUL 
#define o_EQLC &so_EQLC 
#define o_EDL &so_EDL 

#define o_std_DO &so_std_DO 
#define o_std_DC &so_std_DC 
#define o_std_NUD &so_std_NUD 
#define o_movfr_DO &so_movfr_DO 
#define o_movfr_DC &so_movfr_DC 
#define o_movfr_NUD &so_movfr_NUD 

#define o_std_DOR &so_std_DOR 
#define o_std_DCR &so_std_DCR 
#define o_std_NUDR &so_std_NUDR 
#define o_movfr_DOR &so_movfr_DOR 
#define o_movfr_DCR &so_movfr_DCR 
#define o_movfr_NUDR &so_movfr_NUDR 

#define i_movfr_DCL &si_movfr_DCL 
#define i_movfr_DOL &si_movfr_DOL 
#define i_movfr_EE &si_movfr_EE 
#define i_movfr_DPM &si_movfr_DPM 
#define i_movfr_DCLR &si_movfr_DCLR 
#define i_movfr_DOLR &si_movfr_DOLR 
#define i_movfr_EER &si_movfr_EER 
#define i_movfr_RPM &si_movfr_RPM 


#define i_std_DCL &si_std_DCL 
#define i_std_DOL &si_std_DOL 
#define i_std_EE &si_std_EE 
#define i_std_DPM &si_std_DPM 
#define i_std_DCLR &si_std_DCLR 
#define i_std_DOLR &si_std_DOLR 
#define i_std_EER &si_std_EER 
#define i_std_RPM &si_std_RPM 

#define o_DDAC &so_DDAC 
#define o_UDAC &so_UDAC 
#define o_DPI1C &so_DPI1C 
#define o_DPI2C &so_DPI2C 
#define o_DPI3C &so_DPI3C 
#define o_DPI4C &so_DPI4C 
#define o_DPI5C &so_DPI5C 
#define o_DPI6C &so_DPI6C 

#define o_PI(fl) &so_PIO[fl]
#define o_PIC(fl) &so_PICO[fl]

#define o_UPL(fl) &so_ULO[fl]
#define o_DNL(fl) &so_DLO[fl]
#define o_UPLR(fl) &so_ULRO[fl]
#define o_DNLR(fl) &so_DLRO[fl]

#define o_UAL(fl) &so_UALO[fl]
#define o_DAL(fl) &so_DALO[fl]
#define o_UALR(fl) &so_UALRO[fl]
#define o_DALR(fl) &so_DALRO[fl]
	
#define i_CC(fl) &si_CCI[fl]
#define o_CCL(fl) &so_CCO[fl]
#define i_CCR(fl) &si_CCRI[fl]
#define o_CCRL(fl) &so_CCRO[fl]
#define i_CCS(fl) &si_CCSI[fl]
#define i_RCCS(fl) &si_RCCSI[fl]
#define i_OCS(fl) &si_OCSI[fl]
#define i_ORS(fl) &si_ORSI[fl]

#define i_RM_CC(fl) &si_RM_CCI[fl]
#define o_RM_CCL(fl) &so_RM_CCO[fl]
#define i_RM_CCR(fl) &si_RM_CCRI[fl]
#define o_RM_CCRL(fl) &so_RM_CCRO[fl]

#define o_SCCL(fl) &so_SCCO[fl]
#define o_SCCRL(fl) &so_SCCRO[fl]

#define i_DSI &si_DSI 
#define i_RDSI &si_RDSI 
#define i_CEOF &si_CEOF 
#define i_CEOFR &si_CEOFR 
#define o_CEOFL &so_CEOFL 
#define i_ATHCD &si_ATHCD 

#define i_CSP1 &si_CSPI1 
#define i_CSP2 &si_CSPI2 
#define i_CSP3 &si_CSPI3 
#define o_CSP1 &so_CSPO1
#define o_CSP2 &so_CSPO2
#define o_CSP3 &so_CSPO3

//ZYM 10/16/2017
//input
#define i_CTi0 &si_CTi0 
#define i_CTi1 &si_CTi1 
#define i_CTi2 &si_CTi2 
#define i_CTi3 &si_CTi3 
#define i_CTi4 &si_CTi4 
#define i_CTi5 &si_CTi5 
#define i_CTi6 &si_CTi6 
#define i_CTi7 &si_CTi7 
#define i_CTi8 &si_CTi8 
#define i_CTi9 &si_CTi9 
#define i_CTi10 &si_CTi10 
#define i_CTi11 &si_CTi11 

//output
#define o_CTo0 &so_CTo0 
#define o_CTo1 &so_CTo1 
#define o_CTo2 &so_CTo2 
#define o_CTo3 &so_CTo3 
#define o_CTo4 &so_CTo4 
#define o_CTo5 &so_CTo5 
#define o_CTo6 &so_CTo6 
#define o_CTo7 &so_CTo7 
#define o_CTo8 &so_CTo8 
#define o_CTo9 &so_CTo9 
#define o_CTo10 &so_CTo10 
#define o_CTo11 &so_CTo11 

//WBG 2018-5-05
//input
#define i_SWi0 &si_SWi0 
#define i_SWi1 &si_SWi1 
#define i_SWi2 &si_SWi2 
#define i_SWi3 &si_SWi3 
#define i_SWi4 &si_SWi4 
#define i_SWi5 &si_SWi5 
#define i_SWi6 &si_SWi6 
#define i_SWi7 &si_SWi7 
#define i_SWi8 &si_SWi8 
#define i_SWi9 &si_SWi9 
#define i_SWi10 &si_SWi10 
#define i_SWi11 &si_SWi11 

//output
#define O_LDO0 &SO_LDO0 
#define O_LDO1 &SO_LDO1 
#define O_LDO2 &SO_LDO2 
#define O_LDO3 &SO_LDO3 
#define O_LDO4 &SO_LDO4 
#define O_LDO5 &SO_LDO5 
#define O_LDO6 &SO_LDO6 
#define O_LDO7 &SO_LDO7 
#define O_LDO8 &SO_LDO8 
#define O_LDO9 &SO_LDO9 
#define O_LDO10 &SO_LDO10 
#define O_LDO11 &SO_LDO11

//*******************************
//  Aux Car Station
//*******************************

#define i_XCC(fl) &si_XCCI[fl]
#define o_XCCL(fl) &so_XCCO[fl]
#define i_XCCR(fl) &si_XCCRI[fl]
#define o_XCCRL(fl) &so_XCCRO[fl]

#define i_XDOB &si_XDOB 
#define i_XDCB &si_XDCB 
#define i_XDOBR &si_XDOBR 
#define i_XDCBR &si_XDCBR 

#define i_XALRM &si_XALRM 
#define i_XHBE &si_XHBE 
#define i_XED &si_XED 
#define i_XINDC &si_XINDC 
#define i_XALRMR &si_XALRMR 
#define i_XHBER &si_XHBER 
#define i_XEDR &si_XEDR


#define i_UC(fl) &si_UC[fl]
#define o_UCL(fl) &so_UCL[fl]
#define i_DC(fl) &si_DC[fl]
#define o_DCL(fl) &so_DCL[fl]

#define i_RUC(fl) &si_RUC[fl]
#define o_RUCL(fl) &so_RUCL[fl]
#define i_RDC(fl) &si_RDC[fl]
#define o_RDCL(fl) &so_RDCL[fl]

#define i_IRUC(fl) &si_IRUC[fl]
#define o_IRUCL(fl) &so_IRUCL[fl]
#define i_IRDC(fl) &si_IRDC[fl]
#define o_IRDCL(fl) &so_IRDCL[fl]

#define i_RIRUC(fl) &si_RIRUC[fl]
#define o_RIRUCL(fl) &so_RIRUCL[fl]
#define i_RIRDC(fl) &si_RIRDC[fl]
#define o_RIRDCL(fl) &so_RIRDCL[fl]

#define i_TRUC(fl) &si_TRUC[fl]
#define o_TRUCL(fl) &so_TRUCL[fl]
#define i_TRDC(fl) &si_TRDC[fl]
#define o_TRDCL(fl) &so_TRDCL[fl]

#define i_XUC(fl) &si_XUC[fl]
#define o_XUCL(fl) &so_XUCL[fl]
#define i_XDC(fl) &si_XDC[fl]
#define o_XDCL(fl) &so_XDCL[fl]

#define i_XRUC(fl) &si_XRUC[fl]
#define o_XRUCL(fl) &so_XRUCL[fl]
#define i_XRDC(fl) &si_XRDC[fl]
#define o_XRDCL(fl) &so_XRDCL[fl]


#define i_CB(fl) &si_CB[fl]
#define o_CBL(fl) &so_CBL[fl]
#define i_RCB(fl) &si_RCB[fl]
#define o_RCBL(fl) &so_RCBL[fl]

#define i_UB(fl) &si_UB[fl]
#define o_UBL(fl) &so_UBL[fl]
#define i_DB(fl) & si_DB[fl]
#define o_DBL(fl) &so_DBL[fl]

#define i_URB(fl) &si_URB[fl]
#define o_URBL(fl) &so_URBL[fl]
#define i_DRB(fl) &si_DRB[fl]
#define o_DRBL(fl) & so_DRBL[fl]

#define i_CB_SR(fl) &si_CB_SR[fl]
#define o_CB_SRL(fl) &so_CB_SRL[fl]
#define i_RCB_SR(fl) &si_RCB_SR[fl]
#define o_RCB_SRL(fl) &so_RCB_SRL[fl]

#define i_VIP(fl) &si_VIP[fl]
#define o_VIPL(fl) &so_VIPL[fl]

#define i_RVIP(fl) &si_RVIP[fl]
#define o_RVIPL(fl) &so_RVIPL[fl]

// Car Call lockout from the group
#define i_GCSEC &si_GCSEC 
#define i_RGCSC &si_RGCSC 
#define i_GCS(fl) &si_GCS[fl]
#define i_RGCS(fl) &si_RGCS[fl]

// Car Call Security Override from the group
#define i_GCSO &si_GCSO 
#define i_RGCSO &si_RGCSO 
#define i_GOS(fl) &si_GOS[fl]
#define o_GOSO(fl) &so_GOSO[fl]
#define i_RGOS(fl) &si_RGOS[fl]
#define o_RGOSO(fl) &so_RGOSO[fl]

// Floor (hall and car call) lockout from the group
#define i_FLSEC &si_FLSEC 
#define i_RFLSC &si_RFLSC 
#define i_FLS(fl) &si_FLS[fl]
#define i_RFLS(fl) &si_RFLS[fl]

// Hall Call lockout from the group
#define i_HSEC &si_HSEC 
#define i_RHSEC &si_RHSEC 
#define i_HCS(fl) &si_HCS[fl]
#define i_RHCS(fl) &si_RHCS[fl]

// Hall Call lockout from the group
#define i_SHSEC &si_SHSEC 
#define i_RSHSEC &si_RSHSEC 
#define i_SHS(fl) &si_SHS[fl]
#define i_RSHS(fl) &si_RSHS[fl]

// Serial Hall Call lockout Light from the group
#define o_SHSL(fl) &so_SHSL[fl]
#define o_RSHSL(fl) &so_RSHSL[fl]

//  Hugs security inputs
#define i_HGSEC &si_HUGSEC
#define i_RHGSEC &si_RHUGSEC 
#define i_HGS(fl) &si_HGS[fl]
#define i_RHGS(fl) &si_RHGS[fl]


// Sabbath Floor Enable from Group
#define i_SABF &si_SABF 
#define i_RSABF &si_RSABF 
#define i_SBF(fl) &si_SBF[fl]
#define i_SBR(fl) &si_SBR[fl]


// Group I/O
#define i_ICR &si_ICR 
#define i_ICR2 &si_ICR2 
#define i_RICR &si_RICR 

#define i_VIPEN &si_VIPEN 
#define i_RVIPEN &si_RVIPEN 

#define i_LUC &si_LUC
#define o_LUCL &so_LUCL

#define i_CASEC &si_CASEC 
#define i_APRK &si_APRK 
#define i_NCU &si_NCU 
#define i_ALBY &si_ALBY 

#define i_AUTEP &si_AUTEP
#define i_RECEN &si_RECEN
#define i_RUNEN &si_RUNEN
#define o_RUNCT &so_RUNCT
#define o_EPRCLF &so_EPRCLF

#define i_CBSR &si_CBSR
#define i_RCBSR &si_RCBSR
#define i_TR &si_TR

#define i_SECF1 &si_SECF1
#define i_SECF2 &si_SECF2
#define i_SECF3 &si_SECF3
#define i_TMREN &si_TMREN
#define o_GSERV &so_GSERV

#define o_PRK(el) &so_PRK[el]
#define o_OP(el) &so_OP[el]
#define o_SEL(el) &so_SEL[el]

#define i_NCA &si_NCA

#define i_CYCLE &si_CYCLE
#define o_CYCLO &so_CYCLO
#define i_VPECC &si_VPECC

#define i_GSP1 &si_GSPI1 
#define i_GSP2 &si_GSPI2 
#define i_GSP3 &si_GSPI3 
#define o_GSP1 &so_GSPO1
#define o_GSP2 &so_GSPO2
#define o_GSP3 &so_GSPO3

//ZYM 10/9/2017
/*
#define i_TESi0 &si_TESi0 
#define i_TESi1 &si_TESi1 
#define i_TESi2 &si_TESi2 
#define i_TESi3 &si_TESi3 
#define i_TESi4 &si_TESi4 
#define i_TESi5 &si_TESi5 
#define i_TESi6 &si_TESi6 
#define i_TESi7 &si_TESi7 
#define i_TESi8 &si_TESi8 
#define i_TESi9 &si_TESi9 
#define i_TEi10 &si_TEi10 
#define i_TEi11 &si_TEi11 

#define o_TESo0 &so_TESo0
#define o_TESo1 &so_TESo1
#define o_TESo2 &so_TESo2
#define o_TESo3 &so_TESo3
#define o_TESo4 &so_TESo4
#define o_TESo5 &so_TESo5
#define o_TESo6 &so_TESo6
#define o_TESo7 &so_TESo7
#define o_TESo8 &so_TESo8
#define o_TESo9 &so_TESo9
#define o_TEo10 &so_TEo10
#define o_TEo11 &so_TEo11
*/

//ZYM 10/9/2017
#define i_GRi0 &si_GRi0 
#define i_GRi1 &si_GRi1 
#define i_GRi2 &si_GRi2 
#define i_GRi3 &si_GRi3 
#define i_GRi4 &si_GRi4 
#define i_GRi5 &si_GRi5 
#define i_GRi6 &si_GRi6 
#define i_GRi7 &si_GRi7 
#define i_GRi8 &si_GRi8 
#define i_GRi9 &si_GRi9 
#define i_GRi10 &si_GRi10 
#define i_GRi11 &si_GRi111 

#define o_GRo0 &so_GRo0
#define o_GRo1 &so_GRo1
#define o_GRo2 &so_GRo2
#define o_GRo3 &so_GRo3
#define o_GRo4 &so_GRo4
#define o_GRo5 &so_GRo5
#define o_GRo6 &so_GRo6
#define o_GRo7 &so_GRo7
#define o_GRo8 &so_GRo8
#define o_GRo9 &so_GRo9
#define o_GRo10 &so_GRo10
#define o_GRo11 &so_GRo11

/* 

Revision History:

12/4/14 v7.1.22 mhd		1. Corrected definition of all cross assignment variables from pointing to the standard hall calls such as
						   "#define i_XUC(fl) &si_UC[fl]" to "#define i_XUC(fl) &si_XUC[fl]".
						   
*/