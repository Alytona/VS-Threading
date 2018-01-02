# include "stdafx.h"
# include "Threads.h"

// ������������ ���������� ������ ������
TTransmittingBuffer::TTransmittingBuffer( int size ) {

	iSize = size;

	// ������������ ��������� ������
	pBuffer = new double[ iSize ];

	iBeginPos = iEndPos = 0;

	// ������� �������
	rMutex = gcnew Mutex;
}
//-------------------------------------------------------------------

TTransmittingBuffer::~TTransmittingBuffer() {
	
	delete[] pBuffer;
	delete rMutex;
}
//-------------------------------------------------------------------


// �����, ������� ������ ����� ������
void TTransmittingBuffer::init() {

	// ������� ��������� ������� ����� ��������� �����
	rMutex->WaitOne();

	iBeginPos = iEndPos = 0;

	// ����������� ������� ����� ���������� ����
	rMutex->ReleaseMutex();
}
//-------------------------------------------------------------------

// ����� ���������� ������� ������������� ����������� ������
float TTransmittingBuffer::fillingInPercent() {

	int bpos = 0, epos = 0, buffer_size = 0;

	// ������� ��������� ������� ����� ��������� �����
	rMutex->WaitOne();

	// ���������� �������� �� ����������� ����������, ����� �������������� ����� ������� ��������
	bpos = iBeginPos;
	epos = iEndPos;

	// ����������� ������� ����� ���������� ����
	rMutex->ReleaseMutex();

	// ��������� ������������� ������
	if (bpos <= epos) buffer_size = epos - bpos;
	else buffer_size = (iSize - bpos) + epos;

	return (float)(buffer_size)*100.0/(float)iSize;
}
//-------------------------------------------------------------------

// ����� ���������� ������� �������� � ��������� �������, 
// ����������� ��������� ��������� ������� �� ��������� ������ �������
double TTransmittingBuffer::peekOneValue() {

	double value = 0;

	// ������� ��������� ������� ����� ��������� �����
	// ��������� ��������� ���� ���, ��� ������������ ��������� �������
	rMutex->WaitOne();

	// ������������� ��������� ����������, ����� ������� ����� ���� �������������� �����������
	try {

		value = pBuffer[iBeginPos];
		if (iBeginPos == iSize-1) iBeginPos = 0;
		else 
			iBeginPos++;

	} catch (...) {}

	// ����������� ������� ����� ���������� ����
	rMutex->ReleaseMutex();

	return value;
}
//-------------------------------------------------------------------

// ����� ����� �������� � ����� ������, 
// ����������� ��������� �������� ������� �� ��������� ������
// ���������� false, ���� ����� �����

bool TTransmittingBuffer::addOneValue( double value ) {

	bool res = false;

	// BeginPos - ������ ����������� ������� � ������ (0-499999)
	// EndPos - ������ ������������� ������� � ������ (0-499999)
	// EndPos == BeginPos - ����� ����
	// EndPos == 499999 && BeginPos == 0 || EndPos == BeginPos-1 - ����� �����

	// ������� ��������� ������� ����� ��������� �����
	// ��������� ��������� ���� ���, ��� ������������ ��������� �������
	rMutex->WaitOne();

	// ������������� ��������� ����������, ����� ������� ����� ���� �������������� �����������
	try {

		// ���������� �������� ����� iBeginPos
		if (iEndPos >= iBeginPos && iEndPos <= iSize-1) {
			
			pBuffer[iEndPos] = value;

			if (iEndPos == iSize-1) iEndPos = 0;
			else iEndPos++;

			res = true;
		}
		else
			// ���������� �������� ����� iBeginPos
			if (iEndPos < iBeginPos-1) {

				pBuffer[iEndPos] = value;
				iEndPos++;

				res = true;
			}

	// ��������� ����������
	} catch (...) {}

	// ����������� ������� ����� ���������� ����
	rMutex->ReleaseMutex();

	return res;
}
//-------------------------------------------------------------------

// ���������� true, ���� ����� �����
bool TTransmittingBuffer::isFull()
{
	bool res = true;

	// ������� ��������� ������� ����� ��������� �����
	rMutex->WaitOne();

	// ������������� ��������� ����������, ����� ������� ����� ���� �������������� �����������
	try {

		res = (iEndPos == iSize-1 && iBeginPos == 0 || iEndPos == iBeginPos-1); 

	// ��������� ����������
	} catch (...) {}

	// ����������� ������� ����� ���������� ����
	rMutex->ReleaseMutex();

	return res;
}
//-------------------------------------------------------------------

// ���������� true, ���� ����� ����
bool TTransmittingBuffer::isEmpty()
{
	bool res = true;

	// ������� ��������� ������� ����� ��������� �����
	rMutex->WaitOne();

	// ������������� ��������� ����������, ����� ������� ����� ���� �������������� �����������
	try {

		res = (iBeginPos == iEndPos); 

	// ��������� ����������
	} catch (...) {}

	// ����������� ������� ����� ���������� ����
	rMutex->ReleaseMutex();

	return res;
}
//-------------------------------------------------------------------

// ������������ ���������� ������ �� �����, ��������� �����
TTransmitterThread::TTransmitterThread( TTransmittingBuffer ^buffer )
{
	refBuffer = buffer;

	rPauseMutex = gcnew Mutex;
	rQuantityMutex = gcnew Mutex;
} 
//-------------------------------------------------------------------

TTransmitterThread::~TTransmitterThread()
{
	delete rPauseMutex;
	delete rQuantityMutex;
}

// ����� ��� ������ ���������� ��������, ������� ����� ������� ��������
// ����������� � ������ ������� �����.
int TTransmitterThread::getQuantity()
{
	int quantity = 0;

	// ������� ��������� ������� ����� ��������� �����
	rQuantityMutex->WaitOne();

	// ��������� ��������� ���� ���, ��� ���������� ��������� � ���������� ��������, ������� ����� ������� ��������
	quantity = iQuantity;

	// ����������� ������� ����� ���������� ����
	rQuantityMutex->ReleaseMutex();

	return quantity;
}
//-------------------------------------------------------------------

// ������ ������, ���������� �������� ��������
void TTransmitterThread::start( int interval )
{
	iInterval = interval;

	// ��������� ���� ������
	outFile = IO::File::CreateText( ".\\out" );

	// ������� ����� ����������� � ��������� ���
	threadTransmitter = gcnew System::Threading::Thread( 
		gcnew ThreadStart( this, &TTransmitterThread::threadCode ) );
	threadTransmitter->Start();
}
//-------------------------------------------------------------------

// ������������ ������
void TTransmitterThread::pause()
{
	// ����������� ������� ������������ ������ ��������
	rPauseMutex->WaitOne();

	// ���������� ����� ����� �� ����
	outFile->Flush();
}
//-------------------------------------------------------------------

// ������������� ������ ����� �����
void TTransmitterThread::resume()
{
	// ����������� ������� ������������ ������ ��������
	rPauseMutex->ReleaseMutex();
}
//-------------------------------------------------------------------

// ������� ������
void TTransmitterThread::stop()
{
	// ��������� ����� ��������
	threadTransmitter->Abort();
	delete threadTransmitter;

	// ���������� ����� ����� �� ���� � ��������� ����
	outFile->Flush();
	outFile->Close();
}
//-------------------------------------------------------------------

// ����� ��������
void TTransmitterThread::threadCode()
{
	// �������������� ���
	Random random_number;

	int quantity = 0;

	// ������, ���� ����� �� ������� �� ��������� ������
	while( 1 )
	{
		// ���������� ��������� ���������� ��������, ������� ����� ������� ��������
		quantity = random_number.Next( 10000, 100000 );
		// ������� ��������� ������� ����� ��������� �����
		// ��������� ��������� ���, ��� ���������� ��������� � ���������� ��������, ������� ����� ������� ��������
		rQuantityMutex->WaitOne();
		iQuantity = quantity;
		// ����������� ������� ����� ���������� ����
		rQuantityMutex->ReleaseMutex();

		// "����������" ��������� ���������� ��������
		for (int i=0; i<quantity; i++)
		{
			// ���� ����� ��������� ���������� (����� ����), ���� ����� �� �������
			while (refBuffer->isEmpty())
				// ��������� ��������, ������ ����, 
				// ��� ��� ���� �������������� �������� � 100%-� �������� ����������
				System::Threading::Thread::Sleep( 10 );

			// ��������� �������� � ����
			outFile->WriteLine( String::Format( "{0,5:N6}", refBuffer->peekOneValue() ) );

			// ��������� �����
			// ���� ���������� ��������� �������, �� ��� �������� � �������� ���� �� �� �����������
			rPauseMutex->WaitOne();
			// ����� ������� �������� ��� ���������� ����������
			rPauseMutex->ReleaseMutex();
		}

		// ���������� �� ���� ����� �����
		outFile->Flush();
		
		// ����������� �������� ��������
		System::Threading::Thread::Sleep( iInterval );
	}
}
//-------------------------------------------------------------------

// ������������ ���������� ������ �� �����, ��������� �����
TGeneratorThread::TGeneratorThread( TTransmittingBuffer ^buffer )
{
	refBuffer = buffer;

	// ������� ������� ������������ ������ ����������
	rPauseMutex = gcnew Mutex;
}
//-------------------------------------------------------------------

TGeneratorThread::~TGeneratorThread()
{
	delete rPauseMutex;
}
//-------------------------------------------------------------------

// ������ ����������, ���������� ������������� �������
void TGeneratorThread::start( int discreet )
{
	iDiscreet = discreet;

	// ������������� ������ ����� ��������
	refBuffer->init();

	// ������� ����� ���������� � ��������� ���
	threadGenerator = gcnew System::Threading::Thread( 
		gcnew ThreadStart( this,  &TGeneratorThread::threadCode ) );
	threadGenerator->Start();
}
//-------------------------------------------------------------------

// ������������ ������
void TGeneratorThread::pause()
{
	// ����������� ������� ������������ ������ ��������
	rPauseMutex->WaitOne();
}
//-------------------------------------------------------------------

// ������������� ������ ����� �����
void TGeneratorThread::resume()
{
	// ����������� ������� ������������ ������ ��������
	rPauseMutex->ReleaseMutex();
}
//-------------------------------------------------------------------

// ������� ������
void TGeneratorThread::stop()
{
	// ��������� �����
	threadGenerator->Abort();

	// � ������� ���
	delete threadGenerator;
}
//-------------------------------------------------------------------

// ������� ���������� ���������� �������� 
double nextValue( double value, double &delta ) {

	// ���� ������� � ��������, �������� ���������� ������������ ����������
	if (delta > 0 && value < 1.0 || delta < 0 && value > 0.0);
	else
	{
		// ��� ���������� �������� �������� ��� ������� ������� ������ ���� � ����������
		if (value > 1.0) value = 1.0;
		if (value < 0.0) value = 0.0;
		delta =- delta;
	}

	// ����� ��������� �������� ������� ����������� �� 9-�� ����� �� �������� ����������
	return System::Math::Round( value + delta, 9 );
}
//-------------------------------------------------------------------

// ����� ����������
void TGeneratorThread::threadCode()
{
	double 
		value = 0.0,					// ��������� �������� ������� 
		delta = 0.5/(double)iDiscreet;	// ����������, ������� �� �������������
	
	// ����, ���� ����� �� ������� �� ��������� ������
	while (1) {

		// ��������� �����
		// ���� ���������� ��������� �������, �� ��� �������� � �������� ���� �� �� �����������
		rPauseMutex->WaitOne();
		// ����� ������� �������� ��� ���������� ����������
		rPauseMutex->ReleaseMutex();

		// ���� ����� �����, ���� ����� �� �����������
		if (refBuffer->isFull())
			// ��������� ��������, ������ ����, 
			// ��� ��� ���� �������������� �������� � 100%-� �������� ����������
			System::Threading::Thread::Sleep( 10 ); 

		// ���� ����� �� �����, �� ���������� ��������
		else {
			refBuffer->addOneValue( value );
			value = nextValue( value, delta );
		}
	}
}
//-------------------------------------------------------------------
