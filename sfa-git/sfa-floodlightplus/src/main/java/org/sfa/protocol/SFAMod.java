package org.sfa.protocol;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import net.floodlightcontroller.core.IOFSwitch;

import org.openflow.protocol.OFMessage;
import org.openflow.protocol.OFType;
import org.openflow.util.U16;
import org.slf4j.Logger;





/*
	Author : eric
	Description : create the sfa at mode  msg
	
	msg pattern:
	
	of_header:
		version(4)
		type
		length
		xid
	sfa_body:
		SFAStModType 
		SFASt.count
		... count of below...
		SFASt.tableid
		SFASt.matchmap
		SFASt.status
		..........
*/

public class SFAMod extends OFMessage {
    public static int MINIMUM_LENGTH = 32;
    

    protected int appid;
    protected int count;
    protected int bflag; 	//1---st 0---at -1---null
    protected static Logger logger;
    
    public class STMOD_DATA{
    	SFAModType type;
    	STDATA stdata;
    
    	public STMOD_DATA( SFAModType t , STDATA s){
    		type = t ;
    		stdata = s;
    	}
    	
    }
    
    public class ATMOD_DATA{
    	SFAModType type;
    	ATDATA atdata;
    
    	public ATMOD_DATA( SFAModType t , ATDATA a){
    		type = t ;
    		atdata = a;
    	}
    	
    }
    protected List<STMOD_DATA> stdatas = new ArrayList<STMOD_DATA>();
    protected List<ATMOD_DATA> atdatas = new ArrayList<ATMOD_DATA>();
    
    public SFAMod(){
    	appid = 0 ;
    	count = 0 ;
    	bflag = -1;
    }
    
    public SFAMod addMsg( int id ,SFAModType t, Object ob){
    	if( ob.getClass().getName() == "org.sfa.protocol.STDATA"){
    		this.type = OFType.SFA_ST_MOD;
    		this.setSTMod(id, t, (STDATA)ob);   		
    	}else if( ob.getClass().getName() =="org.sfa.protocol.ATDATA"){
    		this.type = OFType.SFA_AT_MOD;
    		this.setATMod(id, t, (ATDATA)ob);
    	}  	
		return this;   	
    }
    
    public SFAMod setSTMod( int id , SFAModType t , STDATA s){
    	
    	appid = id;
    	bflag = 1;
    	stdatas.add(new STMOD_DATA(t,s));
    	count = stdatas.size();
    	return this;
    	
    }
    
    public SFAMod setATMod( int id , SFAModType t, ATDATA a){
    	appid = id;
    	bflag = 0;
    	atdatas.add(new ATMOD_DATA(t,a));
    	count = stdatas.size();
    	return this;
    }

    @Override
    public void readFrom(ByteBuffer data) {
        super.readFrom(data);
        //done nothing 
    }

    @Override
    public void writeTo(ByteBuffer data) {
//    	if( bflag == 1) this.type = OFType.SFA_ST_MOD;
//    	else if( bflag == 0) this.type = OFType.SFA_AT_MOD;
//    	else System.out.println("Error SFA MOD message!!");
    	
        super.writeTo(data);
        if( bflag == -1 );
        	 //-throw new RuntimeException("--------SFAStMod  ERROR! ---------");
        this.computeLength();
        System.out.println("Length=" + length);
        data.putInt(appid);
        data.putInt(count);
        
        if( bflag == 1){
        	for(STMOD_DATA tmp : stdatas ){
        		data.putInt(tmp.type.getValue());
        		System.out.println("ST data: " + data.toString());
        		tmp.stdata.WriteTo(data);
        	}
        	
        }else if (bflag == 0){
        	for(ATMOD_DATA tmp : atdatas){
        		data.putInt(tmp.type.getValue());
        		System.out.println("AT data: " + data.toString());
        		tmp.atdata.WriteTo(data);
        	}
        }
        else{
        }

    }
    
    public boolean sendmsg( IOFSwitch sw ){
    	 try {
		     sw.write(this, null);
//		     logger.info("mod msg send !");
		     System.out.println("mod msg sent!");
		     return true;
		 } catch (IOException e) {
//		     logger.error("Failed to write mod msg to siwtch");
			 System.out.println("Fained to write mod msg to switch");
		     return false;
		 }
    }

    /* (non-Javadoc)
     * @see org.openflow.protocol.OFMessage#computeLength()
     */
    @Override
    public void computeLength() {
    	short len = 8;
    	if( bflag == -1 );
       	 //throw new RuntimeException("-----------SFA MOD ERROR!-------- ");
    		//System.out.println("SFA MOD ERROR");
    	
    	if( bflag ==1 ){
    		for(STMOD_DATA tmp : stdatas ){
    			len = (short) (len + 4 + tmp.stdata.getByteLength());
    		}
    		this.length = (short) (len+4+4);
    		
    	}else if( bflag == 0 ){
    		for(ATMOD_DATA tmp : atdatas ){
    			len = (short) (len + 4 + tmp.atdata.getByteLength());
    		}
    		this.length = (short) (len+4+4);   		
    	}
    	else{}
    }
}