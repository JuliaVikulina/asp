/*
 * CDeviceFactory.h
 *
 *  Created on: 10 февр. 2016 г.
 *      Author: alex
 */

#ifndef CDEVICEFACTORY_H_
#define CDEVICEFACTORY_H_

#include "devices/CBaseDevice.h"
#include "abstract/ShlagbaumAbstract.h"
#include "abstract/BsnsLogic.h"

class CDeviceFactory
{
	CDeviceFactory() {}

	boost::mutex mut;

public:

	static CDeviceFactory& getFactory()
	{
		static CDeviceFactory* ptr = nullptr;

		boost::mutex::scoped_lock(mut);

		if (ptr == nullptr) ptr = new CDeviceFactory;

		return *ptr;
	}


	CAbstractDevice* deviceFactory(const std::string& abstractName, const std::string& devName)
	{
		std::vector<CAbstractDevice*> devs;

		CAbstractDevice* dev = nullptr;

		dev = createAbstractDevice<AbstractShlagbaum>(abstractName, devName);
		if (dev) devs.push_back(dev);

		dev = createAbstractDevice<BsnsLogic>(abstractName, devName);
		if (dev) devs.push_back(dev);

//		dev = createAbstractDevice<AbstractPrinter>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractPassSensor>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractPresentSensor>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractDisplay>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractMassStorage>(abstractName, devName);
//		if (dev) devs.push_back(dev);
//		dev = createAbstractDevice<AbstractKKM>(abstractName, devName);
//		if (dev) devs.push_back(dev);

		if (devs.size() > 1)
		{
			std::cout << "ERROR! CDeviceFactory::deviceFactory: detected device configuration error: more than 1 abstract device created" << std::endl;

			for (CAbstractDevice* d: devs)
				delete d;

			return nullptr;
		}

		if (devs.size() == 0)
		{
			std::cout << "ERROR! CDeviceFactory::deviceFactory detected device configuration error: no device created" << std::endl;

			return nullptr;
		}

		std::cout << "CDeviceFactory::deviceFactory created device: " << abstractName << std::endl;

		return devs[0];
	}

private:
	template<class T>
	const std::string& abstractClassName()
	{
		return T::abstractName;
	}

	template<class T>
	CAbstractDevice* createAbstractDevice(const std::string& abstractName, const std::string& devName)
	{
		CAbstractDevice* dev = nullptr;

		if ( devName.find(T::s_abstractName) != std::string::npos)
		{
			std::cout << "CDeviceFactory::createAbstractDevice: Create abstract device: " << abstractName << std::endl;

			dev = T::createDevice(abstractName, devName);
		}
		else
		{
			std::cout << "ERROR! CDeviceFactory::createAbstractDevice: Abstract device " << abstractName << " not found in device list" << std::endl;
		}

		return dev;
	}

};


#endif /* CDEVICEFACTORY_H_ */
