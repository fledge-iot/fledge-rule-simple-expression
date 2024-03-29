timestamps {
    node("ubuntu18-agent") {
        catchError {
            checkout scm
            dir_exists = sh (
		        script: "test -d 'tests' && echo 'Y' || echo 'N' ",
                returnStdout: true
            ).trim()

            if (dir_exists == 'N'){
                currentBuild.result= 'FAILURE'
                echo "No tests directory found! Exiting."
                return
            }
            try {
                stage("Prerequisites"){
                    // Change to corresponding CORE_BRANCH as required
                    // e.g. FOGL-xxxx, main etc.
                    // Change NOTIFICATION_SERVICE_BRANCH to required branch of fledge-service-notification repository
                    // e.g. FOGL-xxxx, main etc.
                    sh '''
                        CORE_BRANCH='develop'
                        NOTIFICATION_SERVICE_BRANCH='develop'
                        ${HOME}/buildFledge ${CORE_BRANCH} ${WORKSPACE} ${NOTIFICATION_SERVICE_BRANCH}
                    '''
                }
            } catch (e) {
                currentBuild.result = 'FAILURE'
                echo "Failed to build Fledge; required to run the tests!"
                return
            }   

            try { 
                stage("Run Tests"){
                    echo "Executing tests..."
                    sh '''
                        export FLEDGE_ROOT=$HOME/fledge && export NOTIFICATION_SERVICE_INCLUDE_DIRS=$HOME/fledge-service-notification/C/services/notification/include
                        cd tests && cmake . && make && ./RunTests --gtest_output=xml:test_output.xml
                    '''
                    echo "Done."
                }
            } catch (e) {
                result = "TESTS FAILED" 
                currentBuild.result = 'FAILURE'
                echo "Tests failed!"
            }

            try {
                stage("Publish Test Report"){
                    junit "tests/test_output.xml"
                }
            } catch (e) {
                result = "TEST REPORT GENERATION FAILED"
                currentBuild.result = 'FAILURE'
                echo "Failed to generate test reports!"
            }
        }

        stage ("Cleanup"){
            // Add here if any cleanup is required
            echo "Done."
        }
    }
}