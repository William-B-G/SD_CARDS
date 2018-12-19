#define d_GRP_IO 1
#include "global.h"

void group_io (void);
void set_grp_io (void);

//************************************************
// Set Group Inputs and Outputs from Inctime
//************************************************
void set_grp_io (void)
{
	uint16 i;
	uint16 fl_ix;

//	if ((fvars[fvcputiming] & GRP_IO) != 0)
//		SIU.GPDO[Z6_LED].R = 0x01; // tagwbg 14
	
	
	if ((cons[Sabbath] == 2) || (cons[Sabbath] == 6) || (cons[Handicap_HPB] != 0) || ((cons[ids] & 2) != 0) || (cons[hugs] == 1) || (cons[hugs] == 2))
		group_io();
	
	
	if (rdinp(i_EMP) == 1)
		gin_EMP = 1;
	else
		gin_EMP = 0;
	
	if (rdinp(i_EPT) == 1)
		gin_EPT = 1;
	else
		gin_EPT = 0;
	
	if (rdinp(i_ATTHC) == 1)
		gin_ATTHC = 1;
	else
		gin_ATTHC = 0;
	
  
  	if(rdginp(i_HC) == 1)
  	{
  		gin_HCP = 1;
		timers[thct] = 0;
  	}
  	else if ((grcan_dev[c_grcan_SEB_1].online == 0) || (Dispatcher != cons[carnmb]))
		timers[thct] = 0;		// not the group or not communicating with the board
  	
  	if((rdginp(i_HC) == 0) && (timers[thct] > 10))
  	{
	  	// HC fuse blown
  		gin_HCP = 0;
		if ((power_status & ps_HC) == 0)
			record_fault(f_HC_fuse);
		power_status |= ps_HC;
  	}
	else				  
		power_status &= ~ps_HC;

	  	
	if (rdinp(i_SAB) == 1)
		gin_SAB = 1;
	else
		gin_SAB = 0;
	
	if (rdinp(i_SECFM) == 1)
		gin_SECFM = 1;
	else
		gin_SECFM = 0;
	
	if (cons[epgrp] != 0)		// have wired inter-group emergency power
	{
		if (rdginp(i_RECEN) == 1)
			ep_auto_recall_en = 1;
		else
			ep_auto_recall_en = 0;
			
		if (rdginp(i_RUNEN) == 1)
			ep_run_en = 1;
		else
			ep_run_en = 0;
				
	
		if (ep_run_ctl != 0)
			setgoutp(o_RUNCT);
		else
			clrgoutp(o_RUNCT);

	}
	
    if ((cons[EMPop] & 0x10) != 0)
    {
    	
		if(recall_finished == 1)
			setgoutp(o_EPRCLF);
		else
			clrgoutp(o_EPRCLF);
    }
	
    if ((cons[EMPop] & 0x08) != 0)
	{
 		for(i=1;i<=cons[nmbcars];i++)
 		{
			if (ep_prk[i] != 0)
				setgoutp(o_PRK(i));				// EP Car returned and parked output 
			else
				clrgoutp(o_PRK(i));	
			
			if (ep_op[i] != 0)
				setgoutp(o_OP(i));				// EP Car Operation output 
			else
				clrgoutp(o_OP(i));	
			
			if (ep_sel[i] != 0)
				setgoutp(o_SEL(i));				// EP Car selected output 
			else
				clrgoutp(o_SEL(i));	
		}
			
	}
	
	if ((cons[servOUT] & 0x10) != 0)
	{
		if (gout_GSERV == 1)
			setgoutp(o_GSERV);		// Turn on group service output 
		else
			clrgoutp(o_GSERV);		// all cars out of service
		 
	}
	
  	for(i=1; i <= cons[grtopf]; i++)
  	{
  		fl_ix = (i-1)/32;
		if (((g_ucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
		{
	   		if (rdginp(i_UC(i)) == 1)// check to see if up hall call pushed
	   			gc_uc[fl_ix] |= fl_mask[i];
	   		else
	   			gc_uc[fl_ix] &= ~fl_mask[i];
		   		
	   		if ((gc_uc_canc[fl_ix] & fl_mask[i]) != 0)
	   		{
	   			clrgoutp(o_UCL(i));		// group cancelling the call
	#if (Simulator == 1)
				clrginp(i_UC(i));
	#endif	   		
	   		}
	   		else if (((gc_uc[fl_ix] & fl_mask[i]) != 0) || ((gc_uc_set[fl_ix] & fl_mask[i]) != 0))
				setgoutp(o_UCL(i));		// call being set
		}
		
	  	
		if (((g_dcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
		{
	      	if (rdginp(i_DC(i)) == 1)  //check to see if dn hall call pushed
	   			gc_dc[fl_ix] |= fl_mask[i];
	   		else
	   			gc_dc[fl_ix] &= ~fl_mask[i];
		   		
	   		if ((gc_dc_canc[fl_ix] & fl_mask[i]) != 0)
	   		{
				clrgoutp(o_DCL(i));		// group cancelling the call
	#if (Simulator == 1)
				clrginp(i_DC(i));
	#endif	   		
	   		}
	   		else if (((gc_dc[fl_ix] & fl_mask[i]) != 0) || ((gc_dc_set[fl_ix] & fl_mask[i]) != 0))
				setgoutp(o_DCL(i));		// call being set
		}
  	}

	if (grtop_rfl != 0)
	{
		
		for(i=grbot_rfl;i<=grtop_rfl; i++)
		{
			fl_ix = (i-1)/32;
			if (((g_rucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
			{
		   		if (rdginp(i_RUC(i)) == 1)// check to see if up hall call pushed
		   			gc_ruc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_ruc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_ruc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_RUCL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_RUC(i));
		#endif	   		
		   		}
		   		else if (((gc_ruc[fl_ix] & fl_mask[i]) != 0) || ((gc_ruc_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_RUCL(i));		// call being set
			}
		  	
			if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
			{
		      	if (rdginp(i_RDC(i)) == 1)  //check to see if dn hall call pushed
		   			gc_rdc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_rdc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_rdc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
					clrgoutp(o_RDCL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_RDC(i));
		#endif	   		
		   		}
		   		else if (((gc_rdc[fl_ix] & fl_mask[i]) != 0) || ((gc_rdc_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_RDCL(i));		// call being set
			}
	  	}
	}


// Dual serial riser
	if ((cons[Dual_Ser_Riser] != 0) && ((cons[sercom] & 0x20) != 0))
	{
		for(i=1; i <= cons[grtopf]; i++)
	  	{
	  		fl_ix = (i-1)/32;
			if (((pfdsr_msk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
			{
		   		if (rdginp(i_UB(i)) == 1)// check to see if up hall call pushed
		   			gc_uc_dsr[fl_ix] |= fl_mask[i];
		   		else
		   			gc_uc_dsr[fl_ix] &= ~fl_mask[i];
		   			
		   		if ((gc_uc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_UBL(i));
		#if (Simulator == 1)
					clrginp(i_UB(i));	
		#endif
		   		}
		   		else if (((gc_uc[fl_ix] & fl_mask[i]) != 0) || ((gc_uc_set[fl_ix] & fl_mask[i]) != 0))
		   		{
		   			setgoutp(o_UBL(i));
		#if (Simulator == 1)
					setginp(i_UB(i));
		#endif
		   		}
			}
			
		  	
			if (((pfdsr_msk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
			{
		      	if (rdginp(i_DB(i)) == 1)  //check to see if dn hall call pushed
		   			gc_dc_dsr[fl_ix] |= fl_mask[i];
		   		else
		   			gc_dc_dsr[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_dc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_DBL(i));
		#if (Simulator == 1)
					clrginp(i_DB(i));
		#endif	   		
		   		}
		   		else if (((gc_dc[fl_ix] & fl_mask[i]) != 0) || ((gc_dc_set[fl_ix] & fl_mask[i]) != 0))
		   		{
		   			setgoutp(o_DBL(i));
		#if (Simulator == 1)
					setginp(i_DB(i));
		#endif
		   		}
			}
	  	}

		if (grtop_rfl != 0)
		{
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if (((pfdsr_msk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
				{
			   		if (rdginp(i_URB(i)) == 1)// check to see if up hall call pushed
			   			gc_ruc_dsr[fl_ix] |= fl_mask[i];
			   		else
			   			gc_ruc_dsr[fl_ix] &= ~fl_mask[i];
				   		
			   		if ((gc_ruc_canc[fl_ix] & fl_mask[i]) != 0)
			   		{
			   			clrgoutp(o_URBL(i));
			#if (Simulator == 1)
						clrginp(i_URB(i));
			#endif	   		
			   		}
			   		else if (((gc_ruc[fl_ix] & fl_mask[i]) != 0) || ((gc_ruc_set[fl_ix] & fl_mask[i]) != 0))
			   		{
			   			setgoutp(o_URBL(i));
			#if (Simulator == 1)
						setginp(i_URB(i));
			#endif
			   		}
				}
			  	
				if (((pfdsr_msk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
				{
			      	if (rdginp(i_DRB(i)) == 1)  //check to see if dn hall call pushed
			   			gc_rdc_dsr[fl_ix] |= fl_mask[i];
			   		else
			   			gc_rdc_dsr[fl_ix] &= ~fl_mask[i];
				   		
			   		if ((gc_rdc_canc[fl_ix] & fl_mask[i]) != 0)
			   		{
						clrgoutp(o_DRBL(i));
			#if (Simulator == 1)
						clrginp(i_DRB(i));
			#endif	   		
			   		}
			   		else if (((gc_rdc[fl_ix] & fl_mask[i]) != 0) || ((gc_rdc_set[fl_ix] & fl_mask[i]) != 0))
			   		{
			   			setgoutp(o_DRBL(i));
			#if (Simulator == 1)
						setginp(i_DRB(i));
			#endif
			   		}
				}
		  	}
		}
	}


	if ((cons[incrs] != 0) || (cons[Second_Riser] != 0))
	{
		if (rdginp(i_ICR) == 1)
			gc_ir = 1;
		else
			gc_ir = 0;
		
		if (fvars[fvircar2] != 0)
		{
			if (rdginp(i_ICR2) == 1)
				gc_ir_2 = 1;
			else
				gc_ir_2 = 0;
		}
		else
			gc_ir_2 = 0;
		
	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
	  		fl_ix = (i-1)/32;
			if (((g_irucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
			{
		   		if (rdginp(i_IRUC(i)) == 1)// check to see if up hall call pushed
		   			gc_iruc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_iruc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_iruc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_IRUCL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_IRUC(i));
		#endif	   		
		   		}
		   		else if (((gc_iruc[fl_ix] & fl_mask[i]) != 0) || ((gc_iruc_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_IRUCL(i));		// call being set
			}
			
		  	
			if (((g_irdcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
			{
		      	if (rdginp(i_IRDC(i)) == 1)  //check to see if dn hall call pushed
		   			gc_irdc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_irdc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_irdc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
					clrgoutp(o_IRDCL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_IRDC(i));
		#endif	   		
		   		}
		   		else if (((gc_irdc[fl_ix] & fl_mask[i]) != 0) || ((gc_irdc_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_IRDCL(i));		// call being set
			}
	  	}

		if (grtop_rfl != 0)
		{
				
			if (rdginp(i_RICR) == 1)
				gc_rir = 1;
			else
				gc_rir = 0;
		
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if (((g_rirucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
				{
			   		if (rdginp(i_RIRUC(i)) == 1)// check to see if up hall call pushed
			   			gc_riruc[fl_ix] |= fl_mask[i];
			   		else
			   			gc_riruc[fl_ix] &= ~fl_mask[i];
				   		
			   		if ((gc_riruc_canc[fl_ix] & fl_mask[i]) != 0)
			   		{
			   			clrgoutp(o_RIRUCL(i));		// group cancelling the call
			#if (Simulator == 1)
						clrginp(i_RIRUC(i));
			#endif	   		
			   		}
			   		else if (((gc_riruc[fl_ix] & fl_mask[i]) != 0) || ((gc_riruc_set[fl_ix] & fl_mask[i]) != 0))
						setgoutp(o_RIRUCL(i));		// call being set
				}
			  	
				if (((g_rirdcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
				{
			      	if (rdginp(i_RIRDC(i)) == 1)  //check to see if dn hall call pushed
			   			gc_rirdc[fl_ix] |= fl_mask[i];
			   		else
			   			gc_rirdc[fl_ix] &= ~fl_mask[i];
				   		
			   		if ((gc_rirdc_canc[fl_ix] & fl_mask[i]) != 0)
			   		{
						clrgoutp(o_RIRDCL(i));		// group cancelling the call
			#if (Simulator == 1)
						clrginp(i_RIRDC(i));
			#endif	   		
			   		}
			   		else if (((gc_rirdc[fl_ix] & fl_mask[i]) != 0) || ((gc_rirdc_set[fl_ix] & fl_mask[i]) != 0))
						setgoutp(o_RIRDCL(i));		// call being set
				}
		  	}
		}
	}
	
	if (cons[Third_Riser] != 0)
	{
		if (rdginp(i_TR) == 1)
			gc_tr = 1;
		else
			gc_tr = 0;
		
	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
	  		fl_ix = (i-1)/32;
			if (((g_trucmsk[fl_ix] & fl_mask[i]) != 0) && (i < cons[grtopf]))
			{
		   		if (rdginp(i_TRUC(i)) == 1)// check to see if up hall call pushed
		   			gc_truc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_truc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_iruc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_TRUCL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_TRUC(i));
		#endif	   		
		   		}
		   		else if (((gc_truc[fl_ix] & fl_mask[i]) != 0) || ((gc_truc_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_TRUCL(i));		// call being set
			}
			
		  	
			if (((g_trdcmsk[fl_ix] & fl_mask[i]) != 0) && (i > 1))
			{
		      	if (rdginp(i_TRDC(i)) == 1)  //check to see if dn hall call pushed
		   			gc_trdc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_trdc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_trdc_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
					clrgoutp(o_TRDCL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_TRDC(i));
		#endif	   		
		   		}
		   		else if (((gc_trdc[fl_ix] & fl_mask[i]) != 0) || ((gc_trdc_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_TRDCL(i));		// call being set
			}
	  	}

	}
	
	if ((cons[CB_msk] != 0) || (cons[cblue] != 0))
	{
		
	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
	  		fl_ix = (i-1)/32;
			if ((g_cbmsk[fl_ix] & fl_mask[i]) != 0)
			{
		   		if (rdginp(i_CB(i)) == 1)// check to see if up hall call pushed
		   			gc_cb[fl_ix] |= fl_mask[i];
		   		else
		   			gc_cb[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_cb_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_CBL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_CB(i));
		#endif	   		
		   		}
		   		else if (((gc_cb[fl_ix] & fl_mask[i]) != 0) || ((gc_cb_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_CBL(i));		// call being set
			}
			
	  	}

		if (grtop_rfl != 0)
		{
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcbmsk[fl_ix] & fl_mask[i]) != 0)
				{
			   		if (rdginp(i_RCB(i)) == 1)// check to see if up hall call pushed
			   			gc_rcb[fl_ix] |= fl_mask[i];
			   		else
			   			gc_rcb[fl_ix] &= ~fl_mask[i];
				   		
			   		if ((gc_rcb_canc[fl_ix] & fl_mask[i]) != 0)
			   		{
			   			clrgoutp(o_RCBL(i));		// group cancelling the call
			#if (Simulator == 1)
						clrginp(i_RCB(i));
			#endif	   		
			   		}
			   		else if (((gc_rcb[fl_ix] & fl_mask[i]) != 0) || ((gc_rcb_set[fl_ix] & fl_mask[i]) != 0))
						setgoutp(o_RCBL(i));		// call being set
				}
			  	
		  	}
		}
	}
	
	if (cons[CB_SR] != 0) 
	{
	
		if (rdginp(i_CBSR) == 1)
			gc_cb_sr = 1;
		else
			gc_cb_sr = 0;
		
		if (rdginp(i_RCBSR) == 1)
			gc_rcb_sr = 1;
		else
			gc_rcb_sr = 0;
		
	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
	  		fl_ix = (i-1)/32;
			if ((g_cbmsk[fl_ix] & fl_mask[i]) != 0)
			{
		   		if (rdginp(i_CB_SR(i)) == 1)// check to see if up hall call pushed
		   			gc_cbsr[fl_ix] |= fl_mask[i];
		   		else
		   			gc_cbsr[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_cbsr_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_CB_SRL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_CB_SR(i));
		#endif	   		
		   		}
		   		else if (((gc_cbsr[fl_ix] & fl_mask[i]) != 0) || ((gc_cbsr_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_CB_SRL(i));		// call being set
			}
			
	  	}

		if (grtop_rfl != 0)
		{
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcbmsk[fl_ix] & fl_mask[i]) != 0)
				{
			   		if (rdginp(i_RCB_SR(i)) == 1)// check to see if up hall call pushed
			   			gc_rcbsr[fl_ix] |= fl_mask[i];
			   		else
			   			gc_rcbsr[fl_ix] &= ~fl_mask[i];
				   		
			   		if ((gc_rcbsr_canc[fl_ix] & fl_mask[i]) != 0)
			   		{
			   			clrgoutp(o_RCB_SRL(i));		// group cancelling the call
			#if (Simulator == 1)
						clrginp(i_RCB_SR(i));
			#endif	   		
			   		}
			   		else if (((gc_rcbsr[fl_ix] & fl_mask[i]) != 0) || ((gc_rcbsr_set[fl_ix] & fl_mask[i]) != 0))
						setgoutp(o_RCB_SRL(i));		// call being set
				}
			  	
		  	}
		}
	}
	else
	{
		gc_cb_sr = 0;
		gc_rcb_sr = 0;
	}
	
	if ((cons[VIP_msk] != 0) || (cons[VipSvc] != 0))
	{
		if(cons[vip_en] != 0)
		{
		}

	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
	  		fl_ix = (i-1)/32;
			if ((g_vipmsk[fl_ix] & fl_mask[i]) != 0)
			{
		   		if (rdginp(i_VIP(i)) == 1)// check to see if up hall call pushed
		   			gc_vip[fl_ix] |= fl_mask[i];
		   		else
		   			gc_vip[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_vip_canc[fl_ix] & fl_mask[i]) != 0)
		   		{
		   			clrgoutp(o_VIPL(i));		// group cancelling the call
		#if (Simulator == 1)
					clrginp(i_VIP(i));
		#endif	   		
		   		}
		   		else if (((gc_vip[fl_ix] & fl_mask[i]) != 0) || ((gc_vip_set[fl_ix] & fl_mask[i]) != 0))
					setgoutp(o_VIPL(i));		// call being set
			}
			
	  	}

		if (grtop_rfl != 0)
		{
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rvipmsk[fl_ix] & fl_mask[i]) != 0)
				{
			   		if (rdginp(i_RVIP(i)) == 1)// check to see if up hall call pushed
			   			gc_rvip[fl_ix] |= fl_mask[i];
			   		else
			   			gc_rvip[fl_ix] &= ~fl_mask[i];
				   		
			   		if ((gc_rvip_canc[fl_ix] & fl_mask[i]) != 0)
			   		{
			   			clrgoutp(o_RVIPL(i));		// group cancelling the call
			#if (Simulator == 1)
						clrginp(i_RVIP(i));
			#endif	   		
			   		}
			   		else if (((gc_rvip[fl_ix] & fl_mask[i]) != 0) || ((gc_rvip_set[fl_ix] & fl_mask[i]) != 0))
						setgoutp(o_RVIPL(i));		// call being set
				}
			  	
		  	}
		}
	}
	
	// Floor Lockouts

	if (Grp_FL_Lockouts == 1)
	{
		if (rdginp(i_FLSEC) == 1)
		{
			gc_hsec = 1;
			gc_sec = 1;
		}
		else
		{
			gc_hsec = 0;
			gc_sec = 0;
		}
		
		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			if (((g_dcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_ucmsk[fl_ix] & fl_mask[i]) != 0) || ((g_cc_mask[fl_ix] & fl_mask[i]) != 0))
			{
				if (rdginp(i_FLS(i)) == 1)
				{
					gc_hc_sec[fl_ix] |= fl_mask[i];
					gc_cc_sec[fl_ix] |= fl_mask[i];
				}
				else
				{
					gc_hc_sec[fl_ix] &= ~fl_mask[i];
					gc_cc_sec[fl_ix] &= ~fl_mask[i];
				}
			}
		}
		
		
		if (grtop_rfl != 0)
		{
			if (rdginp(i_RFLSC) == 1)
			{
				gc_rhsec = 1;
				gc_rsec = 1;
			}
			else
			{
				gc_rhsec = 0;
				gc_rsec = 0;
			}
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_rucmsk[fl_ix] & fl_mask[i]) != 0) || ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0))
				{
					if (rdginp(i_RFLS(i)) == 1)
					{
						gc_rhc_sec[fl_ix] |= fl_mask[i];
						gc_rcc_sec[fl_ix] |= fl_mask[i];
					}
					else
					{
						gc_rhc_sec[fl_ix] &= ~fl_mask[i];
						gc_rcc_sec[fl_ix] &= ~fl_mask[i];
					}
				}
			}
		}
	}
	
	if (Grp_HC_Lockouts == 1)
	{
		if (rdginp(i_HSEC) == 1)
			gc_hsec = 1;
		else
			gc_hsec = 0;
		
		if (((cons[sercom] & 0x20) != 0) && ((Security_Type[cons[carnmb]] & 0x100) != 0))
		{
			if (rdginp(i_SHSEC) == 1)
				gc_shsec = 1;
			else
				gc_shsec = 0;
		}
		
		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			if (((g_dcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_ucmsk[fl_ix] & fl_mask[i]) != 0))
			{
				if (fvars[fvinvhcsec] == 1)
				{		// invert input signal
					if ((rdginp(i_HCS(i)) == 0) || (((cons[sercom] & 0x20) != 0) && (rdginp(i_SHS(i)) == 1)))
						gc_hc_sec[fl_ix] |= fl_mask[i];
					else
						gc_hc_sec[fl_ix] &= ~fl_mask[i];
				}
				else
				{
					if ((rdginp(i_HCS(i)) == 1) || (((cons[sercom] & 0x20) != 0) && (rdginp(i_SHS(i)) == 1)))
						gc_hc_sec[fl_ix] |= fl_mask[i];
					else
						gc_hc_sec[fl_ix] &= ~fl_mask[i];
				}
			}
			
			if (((cons[sercom] & 0x20) != 0) && ((Security_Type[cons[carnmb]] & 0x100) != 0))
			{
				if ((gc_hc_sec[fl_ix] & fl_mask[i]) != 0)
					setgoutp(o_SHSL(i));
				else
					clrgoutp(o_SHSL(i));
			}
		}
		
		
		if (grtop_rfl != 0)
		{
			if (rdginp(i_RHSEC) == 1)
				gc_rhsec = 1;
			else
				gc_rhsec = 0;
			
			if (((cons[sercom] & 0x20) != 0) && ((Security_Type[cons[carnmb]] & 0x100) != 0))
			{
				if (rdginp(i_RSHSEC) == 1)
					gc_rshsec = 1;
				else
					gc_rshsec = 0;
			}
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if (((g_rdcmsk[fl_ix] & fl_mask[i]) != 0) || ((g_rucmsk[fl_ix] & fl_mask[i]) != 0))
				{
					if (fvars[fvinvhcsec] == 1)
					{		// invert input signal
						if ((rdginp(i_RHCS(i)) == 0) || (((cons[sercom] & 0x20) != 0) && (rdginp(i_RSHS(i)) == 1)))
							gc_rhc_sec[fl_ix] |= fl_mask[i];
						else
							gc_rhc_sec[fl_ix] &= ~fl_mask[i];
					}
					else
					{
						if ((rdginp(i_RHCS(i)) == 1) || (((cons[sercom] & 0x20) != 0) && (rdginp(i_RSHS(i)) == 1)))
							gc_rhc_sec[fl_ix] |= fl_mask[i];
						else
							gc_rhc_sec[fl_ix] &= ~fl_mask[i];
					}
				}

				if (((cons[sercom] & 0x20) != 0) && ((Security_Type[cons[carnmb]] & 0x100) != 0))
				{
					if ((gc_rhc_sec[fl_ix] & fl_mask[i]) != 0)
						setgoutp(o_RSHSL(i));
					else
						clrgoutp(o_RSHSL(i));
				}
			}
		}
	}
	
	
	// Group Car call Overrides from goup
	if (Grp_CC_Override == 1)
	{
		if (rdginp(i_GCSO) == 1)
			gc_sec_ovr = 1;
		else
			gc_sec_ovr = 0;
		
		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				if (rdginp(i_GOS(i)) == 1)
					gc_cc_sec_ovr[fl_ix] |= fl_mask[i];
				else
					gc_cc_sec_ovr[fl_ix] &= ~fl_mask[i];
				
				// Write the outputs
				if (fvars[fvgccovride] == 0)
				{
					if ((gc_cc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0)
						setgoutp(o_GOSO(i));
					else
						clrgoutp(o_GOSO(i));
				}
				
			}
		}
		
		if (grtop_rfl != 0)
		{
			if (rdginp(i_RGCSO) == 1)
				gc_rsec_ovr = 1;
			else
				gc_rsec_ovr = 0;
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					if (rdginp(i_RGOS(i)) == 1)
						gc_rcc_sec_ovr[fl_ix] |= fl_mask[i];
					else
						gc_rcc_sec_ovr[fl_ix] &= ~fl_mask[i];
					
					// Write the outputs
					if (fvars[fvgccovride] == 0)
					{
						if ((gc_rcc_sec_ovr_set[fl_ix] & fl_mask[i]) != 0)
							setgoutp(o_GOSO(i));
						else
							clrgoutp(o_GOSO(i));
					}
				}
			}
		}
	}


	// Car call lockouts from goup
	if (Grp_CC_Lockouts == 1)
	{
		if (rdginp(i_GCSEC) == 1)
			gc_sec = 1;
		else
			gc_sec = 0;
		
		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				if (rdginp(i_GCS(i)) == 1)
					gc_cc_sec[fl_ix] |= fl_mask[i];
				else
					gc_cc_sec[fl_ix] &= ~fl_mask[i];
			}
		}
		
		if (grtop_rfl != 0)
		{
			if (rdginp(i_RGCSC) == 1)
				gc_rsec = 1;
			else
				gc_rsec = 0;
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					if (rdginp(i_RGCS(i)) == 1)
						gc_rcc_sec[fl_ix] |= fl_mask[i];
					else
						gc_rcc_sec[fl_ix] &= ~fl_mask[i];
				}
			}
		}
	}
	
	// Sabbath Floor Enable from Group
	if (cons[Sabbath] == 2)
	{
		if (rdginp(i_SABF) == 1)
			gc_sab_fl = 1;
		else
			gc_sab_fl = 0;
		
		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				if (rdginp(i_SBF(i)) == 1)
					gc_sab_fl_msk[fl_ix] |= fl_mask[i];
				else
					gc_sab_fl_msk[fl_ix] &= ~fl_mask[i];
			}
		}
		
		if (grtop_rfl != 0)
		{
			if (rdginp(i_RSABF) == 1)
				gc_sab_rfl = 1;
			else
				gc_sab_rfl = 0;
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					if (rdginp(i_SBR(i)) == 1)
						gc_sab_rfl_msk[fl_ix] |= fl_mask[i];
					else
						gc_sab_rfl_msk[fl_ix] &= ~fl_mask[i];
				}
			}
		}
	}
	
	// Hugs Security
	if ((cons[hugs] == 1) || (cons[hugs] == 2))
	{
		if (rdginp(i_HGSEC) == 1)
			gc_hugsec = 1;
		else
			gc_hugsec = 0;
		
		for (i=1;i<=cons[grtopf];i++)
		{
			fl_ix = (i-1)/32;
			if ((g_cc_mask[fl_ix] & fl_mask[i]) != 0)
			{
				if (rdginp(i_HGS(i)) == 1)
					gc_hugs_sec[fl_ix] |= fl_mask[i];
				else
					gc_hugs_sec[fl_ix] &= ~fl_mask[i];
			}
		}
		
		if (grtop_rfl != 0)
		{
			if (rdginp(i_RHGSEC) == 1)
				gc_rhugsec = 1;
			else
				gc_rhugsec = 0;
			
			for(i=grbot_rfl;i<=grtop_rfl; i++)
			{
				fl_ix = (i-1)/32;
				if ((g_rcc_mask[fl_ix] & fl_mask[i]) != 0)
				{
					if (rdginp(i_RHGS(i)) == 1)
						gc_rhugs_sec[fl_ix] |= fl_mask[i];
					else
						gc_rhugs_sec[fl_ix] &= ~fl_mask[i];
				}
			}
		}
	}
	
	// Cross Assignment I/O
	
	if ((fvars[fvxasgn] & 0x01) != 0)
	{
	  	for(i=1; i <= cons[grtopf]; i++)
	  	{
	  		fl_ix = (i-1)/32;
			if (i < cons[grtopf])
			{
		   		if (rdginp(i_XUC(i)) == 1)// check to see if up hall call pushed
		   			gc_xuc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_xuc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_xuc_set[fl_ix] & fl_mask[i]) != 0)
					setgoutp(o_XUCL(i));		// transfer call
				else
		   			clrgoutp(o_XUCL(i));		// clearing transfer
			}
			
		  	
			if (i > 1)
			{
		      	if (rdginp(i_XDC(i)) == 1)  //check to see if dn hall call pushed
		   			gc_xdc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_xdc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_xdc_set[fl_ix] & fl_mask[i]) != 0)
					setgoutp(o_XDCL(i));		// trasfer call 
				else
					clrgoutp(o_XDCL(i));		// clearing transfer
			}
	  	}
	}

	if ((grtop_rfl != 0) && ((fvars[fvxasgn] & 0x04) != 0))
	{
			
		for(i=grbot_rfl;i<=grtop_rfl; i++)
		{
			fl_ix = (i-1)/32;
			if (i < cons[grtopf])
			{
		   		if (rdginp(i_XRUC(i)) == 1)// check to see if up hall call pushed
		   			gc_xruc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_xruc[fl_ix] &= ~fl_mask[i];
		   		
		   		if ((gc_xruc_set[fl_ix] & fl_mask[i]) != 0)
					setgoutp(o_RUCL(i));		// trasfer call
				else
		   			clrgoutp(o_XRUCL(i));		// clearing transfer
			}
		  	
			if (i > 1)
			{
		      	if (rdginp(i_XRDC(i)) == 1)  //check to see if dn hall call pushed
		   			gc_xrdc[fl_ix] |= fl_mask[i];
		   		else
		   			gc_xrdc[fl_ix] &= ~fl_mask[i];
			   		
		   		if ((gc_xrdc_set[fl_ix] & fl_mask[i]) != 0)
					setgoutp(o_XRDCL(i));		// trasfer call
				else
					clrgoutp(o_XRDCL(i));		// clearing transfer
			}
	  	}
	}
		

	if ((fvars[fvxasgn] & 0x05) != 0)
	{
		if (rdginp(i_NCA) == 1)		// NO cars available from other group
			gc_nca_xasgn = 1;
		else
			gc_nca_xasgn = 0;
	}

	write_trace(TR_SET_GRP_IO);
	

//	if ((fvars[fvcputiming] & GRP_IO) != 0)
//		SIU.GPDO[Z6_LED].R = 0x00; // tagwbg 14
	
}

//********************************************
//** Control of group io sent to and from car
//********************************************

void group_io (void)
{
	int16 i;
	static uint32 prev_sab_fl_msk[2];
	static uint32 prev_sab_rfl_msk[2];
	static uint32 prev_hugs_sec[2];
	static uint32 prev_rhugs_sec[2];
	int16 update = 0;
	int16 transfer_gio = 0;
	

	if(cons[hugs] == 2)
	{

		gc_hugs_sec[0] = gc_cc_sec[0];
		gc_rhugs_sec[0] = gc_rcc_sec[0];

#if (Tract_HR == 1)

		gc_hugs_sec[1] = gc_cc_sec[1];
		gc_rhugs_sec[1] = gc_rcc_sec[1];

#endif	
	}

	if ((cons[Sabbath] == 2) || (cons[Sabbath] == 6) || (cons[hugs] == 1) || (cons[hugs] == 2))
		transfer_gio |= 1;

	if (transfer_gio != 0)
	{
		if ((transfer_gio & 1) != 0)
		{ 	// Send data from group to car

			if (cons[Sabbath] == 2)
			{
				
				if ((prev_sab_fl_msk[0] != gc_sab_fl_msk[0]) || (prev_sab_fl_msk[1] != gc_sab_fl_msk[1]) ||
					(prev_sab_rfl_msk[0] != gc_sab_rfl_msk[0]) || (prev_sab_rfl_msk[1] != gc_sab_rfl_msk[1]) )
						update = 1;
				prev_sab_fl_msk[0] = gc_sab_fl_msk[0];
				prev_sab_fl_msk[1] = gc_sab_fl_msk[1];
				prev_sab_rfl_msk[0] = gc_sab_rfl_msk[0];
				prev_sab_rfl_msk[1] = gc_sab_rfl_msk[1];
			}
			
			if ((cons[hugs] == 1) || (cons[hugs] == 2))
			{
				if ((prev_hugs_sec[0] != gc_hugs_sec[0]) || (prev_hugs_sec[1] != gc_hugs_sec[1]) ||
					(prev_rhugs_sec[0] != gc_rhugs_sec[0]) || (prev_rhugs_sec[1] != gc_rhugs_sec[1]) )
						update = 1;
				prev_hugs_sec[0] = gc_hugs_sec[0];
				prev_hugs_sec[1] = gc_hugs_sec[1];
				prev_rhugs_sec[0] = gc_rhugs_sec[0];
				prev_rhugs_sec[1] = gc_rhugs_sec[1];
			}
			if ((update == 1) && (cons[nmbcars] > 1))
			{
				for (i=1;i<=cons[nmbcars];i++)
				{
					if ((i != cons[carnmb]) && (gc_dev_online[i] == 1))
						put_pkt_req(22,i,gc_port);
				}
			}
		}

		if (timers[tgpio] > 10)
		{	// update once every seconds
			if (cons[nmbcars] > 1)
			{
				for (i=1;i<=cons[nmbcars];i++)
				{
					if ((i != cons[carnmb]) && (gc_dev_online[i] == 1))
					{
						if ((transfer_gio & 1) != 0)   // Send data from group to car 
							put_pkt_req(22,i,gc_port);
						if ((transfer_gio & 2) != 0)   // Send data from car to group
							put_pkt_req(24,i,gc_port);
					}
				}
			}
			timers[tgpio] = 0;
		}

		if ((transfer_gio & 2) != 0)   // Send data from car to group
		{
			for (i=0;i<=11;i++)
			{
				gpo[i] = cgpo[i];
			}
		}
	} 
}

/*

Revision History

11/12/13 v7.0.28 mhd	1. Replaced cons[carnmb] with 1 to allow any car to be the group.
11/21/13 v7.0.29 mhd	1. Corrected mistake using dcmsk all the time to using ucmsk or dcmsk
12/3/13 v7.0.30 mhd		1. Use parameter fvinvhcsec to invert hall call security inputs.
5/23/14 v7.0.47 mhd		1. For group hc input use rdginp(i_HC), instead of car io read rdinp(i_HC).
8/22/14 v7.1.8 mhd		1. Corrected Second riser down call at top floor.  Changed i > 2 to i > 1.
9/5/14 v7.1.9. mhd 		1. Added cross assignment i/o.
12/4/14 v7.1.22 mhd		1. Aded o_GSERV to turn off cross assignment.if all car out of service.
						2. Corrected typo in cross assignment i/o.
01/05/16 v7.1.59 tb		1. Added logic for Dual Serial Riser i/o's when ((cons[Dual_Ser_Riser] != 0) && ((cons[sercom] & 0x20) != 0)).

*/
