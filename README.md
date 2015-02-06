# GeoSmartSim

## Ingelligent Agent platform for spatial simulations built with Qt

### Authors : Ander Pijoan <ander.pijoan(at)deusto.es>, Oihane Kamara <oihane.esteban(at)deusto.es> & Cruz E. Borges <cruz.borges(at)deusto.es>

GeoSmartSim's main objective is to build an environment where to launch intelligent agents in order to evaluate different urban dynamics. Our Environment consists of three main pieces. 

* On the one hand, a Physical World and a Communication Environment, which come together to form the universe where both passive entities and agents are situated. 

* On the other hand, a Simulation Engine which is in charge of modelling the skills these entities are able to execute (including their communication and perception capabilities).

In fact, the skill sets defined by the Environment is the central point of our vision. Skills are the degree of competence that empowers agents and the way to perceive or propagate actions.

The Entities are the actors that interfere in the simulation. All are derived from a parent class (Agent) containing basic attributes and procedures all objects and agents will inherit, even if they do not make use of them. These entities can be
categorized in two different groups whether they have their own behaviour or
they just react to stimuli.

* Autonomous Agents constitute the traditional agents in a MAS. They are scheduled to follow a certain behaviour and can use the already mentioned skills to interact with other Entities or the Environment. Every agent type will be characterized by the set of skills that are able to use and, additionally, they can have an state given by several private parameters.

* On the other hand, Passive Entities are those entities who, although having some logic inside, just react to other Entities actions. Namely, Passive Entities can provide or receive information from agents or other entities. Please note that passive objects can also make use of skills while honouring their restrictions.

