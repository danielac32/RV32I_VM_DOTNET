public interface IMemoryDevice
{
    uint Read(uint address);
    void Write(uint address, uint value, int width);
}

