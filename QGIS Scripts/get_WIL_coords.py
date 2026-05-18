"""
Author: Wesley Cooke
Date  : 01/29/2025

This file will take a vector geometry from QGIS and process it into 
LONG data types appropriate for use with WIL the autonomous surface vehicle.

Output: output_file.txt -> Defined in the user interface when using the script. 
"""

from qgis.PyQt.QtCore import QCoreApplication
from qgis.core import (QgsProcessing,
                       QgsFeatureSink,
                       QgsProcessingException,
                       QgsProcessingAlgorithm,
                       QgsProcessingParameterFeatureSource,
                       QgsProcessingParameterFeatureSink,
                       QgsProcessingParameterFileDestination,
                       QgsProcessingFeedback)
from qgis import processing


class GetWILPath(QgsProcessingAlgorithm):
    """
    This is an example algorithm that takes a vector layer and
    creates a new identical one.

    It is meant to be used as an example of how to create your own
    algorithms and explain methods and variables used to do it. An
    algorithm like this will be available in all elements, and there
    is not need for additional work.

    All Processing algorithms should extend the QgsProcessingAlgorithm
    class.
    """

    # Constants used to refer to parameters and outputs. They will be
    # used when calling the algorithm from another algorithm, or when
    # calling from the QGIS console.

    INPUT = 'INPUT'
    OUTPUT = 'OUTPUT'

    def tr(self, string):
        """
        Returns a translatable string with the self.tr() function.
        """
        return QCoreApplication.translate('Processing', string)

    def createInstance(self):
        return GetWILPath()
    def name(self):
        """
        Returns the algorithm name, used for identifying the algorithm. This
        string should be fixed for the algorithm, and must not be localised.
        The name should be unique within each provider. Names should contain
        lowercase alphanumeric characters only and no spaces or other
        formatting characters.
        """
        return 'wilpath'

    def displayName(self):
        """
        Returns the translated algorithm name, which should be used for any
        user-visible display of the algorithm name.
        """
        return self.tr('Get WIL Path')

    def group(self):
        """
        Returns the name of the group this algorithm belongs to. This string
        should be localised.
        """
        return self.tr('Example scripts')

    def groupId(self):
        """
        Returns the unique ID of the group this algorithm belongs to. This
        string should be fixed for the algorithm, and must not be localised.
        The group id should be unique within each provider. Group id should
        contain lowercase alphanumeric characters only and no spaces or other
        formatting characters.
        """
        return 'examplescripts'

    def shortHelpString(self):
        """
        Returns a localised short helper string for the algorithm. This string
        should provide a basic description about what the algorithm does and the
        parameters and outputs associated with it..
        """
        return self.tr("Take in a vector layer. Get Point data and multiply by 10^7. Stores output in a .csv")

    def initAlgorithm(self, config=None):
        """
        Here we define the inputs and output of the algorithm, along
        with some other properties.
        """

        # We add the input vector features source. It can have any kind of
        # geometry.
        self.addParameter(
            QgsProcessingParameterFeatureSource(
                self.INPUT,
                self.tr('Input layer'),
                [QgsProcessing.TypeVectorAnyGeometry]
            )
        )

        # We add a File Destination to store our coordinate outputs. 
        self.addParameter(
            QgsProcessingParameterFileDestination(
                self.OUTPUT,
                self.tr('Output layer'),
                self.tr('CSV files (*.csv)'), 
                None, 
                True
            )
        )

    def processAlgorithm(self, parameters, context, feedback):

        """
        Processes the input layer and writes the coordinates to a file.
        """
        # Retrieve the input parameters
        layer = self.parameterAsSource(parameters, self.INPUT, context)
        output_file = self.parameterAsFileOutput(parameters, self.OUTPUT, context)
        
        # Open the output file to write
        with open(output_file, 'w') as file:
            file.write('id, longitude, latitude\n')  # Write header

            # Iterate over features in the input layer
            for feature in layer.getFeatures():
                id = feature['id']
                geom = feature.geometry()
                if geom.isEmpty():
                    continue  # Skip features with no geometry

                # Get the point coordinates
                point = geom.asPoint()
                longitude = point.x() * 10**7
                latitude = point.y()  * 10**7

                # Write the feature ID and coordinates to the file
                file.write(f'{id}, {long(longitude)}, {long(latitude)}\n')

        # Return the output file path as an algorithm result
        return {self.OUTPUT: output_file}
