package macromedia.asc.util;

public class UintNumberConstant extends NumberConstant {

	long val; // easiest representation to keep around 
	
	/*
	 * Java has the feature of converting any double > Integer.MAX_VALUE to Integer.MAX_VALUE.
	 * Take a long in the constructor to avoid surprises in other places if they had to pass an int.
	 */
	public UintNumberConstant(long num) {
		val = num; 
	}
	
	@Override
	public Decimal128 decimalValue() {
		return new Decimal128(val);
	}

	@Override
	public double doubleValue() {
		return val;
	}

	@Override
	public int intValue() {
		return (int)val;
	}

	@Override
	public byte number_type() {
		return NumberUsage.use_uint;
	}

	@Override
	public long uintValue() {
		return val;
	}

	@Override
	public String toString() {
		return String.valueOf(val);
	}

}
