/**
 * @author David Erickson (daviderickson@cs.stanford.edu) - Mar 11, 2010
 */
package org.openflow.protocol.action;

import java.nio.ByteBuffer;

import org.openflow.util.U16;

/**
 * Represents an ofp_action_push
 * @author David Erickson (daviderickson@cs.stanford.edu) - Mar 11, 2010
 */
public class OFActionPushPBB extends OFAction {
    public static int MINIMUM_LENGTH = 8;

    protected short etherType;

    public OFActionPushPBB() {
        super.setType(OFActionType.PUSH_PBB);
        super.setLength((short) MINIMUM_LENGTH);
    }

    public OFActionPushPBB(short etherType) {
        super.setType(OFActionType.PUSH_PBB);
        super.setLength((short) MINIMUM_LENGTH);
        this.etherType = etherType;
    }

    /**
     * @return the etherType
     */
    public short getEtherType() {
        return etherType;
    }

    /**
     * @param etherType the etherType to set
     */
    public OFActionPushPBB setEtherType(short etherType) {
        this.etherType = etherType;
        return this;
    }

    @Override
    public void readFrom(ByteBuffer data) {
        super.readFrom(data);
        this.etherType = data.getShort();
        data.getShort(); //pad
    }

    @Override
    public void writeTo(ByteBuffer data) {
        super.writeTo(data);
        data.putShort(this.etherType);
        data.putShort((short) 0); //pad
    }

    @Override
    public int hashCode() {
        final int prime = 383;
        int result = super.hashCode();
        result = prime * result + etherType;
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (!super.equals(obj)) {
            return false;
        }
        if (!(obj instanceof OFActionPushPBB)) {
            return false;
        }
        OFActionPushPBB other = (OFActionPushPBB) obj;
        if (etherType != other.etherType) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "OFActionPushPBB [etherType="
                + U16.f(etherType) + "]";
    }
}
